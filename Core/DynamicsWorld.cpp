#include "DynamicsWorld.h"
#include "Window.h"
#include <thread>


#include <Bullet/BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <Bullet/BulletMultiThreaded/PlatformDefinitions.h>
#include <Bullet/BulletMultiThreaded/Win32ThreadSupport.h>
#include <Bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>
#include <Bullet/BulletMultiThreaded/btParallelConstraintSolver.h>
#include <Bullet/BulletMultiThreaded/SequentialThreadSupport.h>
#include <Bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.h>

//#define SEQUENTIAL
btThreadSupportInterface* createSolverThreadSupport(int maxNumThreads)
{
	#ifdef SEQUENTIAL
	SequentialThreadSupport::SequentialThreadConstructionInfo tci("solverThreads", SolverThreadFunc, SolverlsMemoryFunc);
	SequentialThreadSupport* threadSupport = new SequentialThreadSupport(tci);
	threadSupport->startSPU();
	#else
	Win32ThreadSupport::Win32ThreadConstructionInfo threadConstructionInfo("solverThreads", SolverThreadFunc, SolverlsMemoryFunc, maxNumThreads);
	Win32ThreadSupport* threadSupport = new Win32ThreadSupport(threadConstructionInfo);
	threadSupport->startSPU();
	#endif
	return threadSupport;
}

namespace Core
{

	DynamicsWorld::DynamicsWorld()
	{
		m_threadSupportSolver = 0;
		m_threadSupportCollision = 0;
		int maxNumOutstandingTasks = std::thread::hardware_concurrency();
		if (maxNumOutstandingTasks == 0)
			maxNumOutstandingTasks = 4;

		if(Settings::Misc::VerboseLogging)
		{
			Debug::Log("Hardware Concurrency: " + std::to_string(std::thread::hardware_concurrency()));
		}

		#ifdef SEQUENTIAL
		SequentialThreadSupport::SequentialThreadConstructionInfo colCI("collision", processCollisionTask, createCollisionLocalStoreMemory);
		m_threadSupportCollision = new SequentialThreadSupport(colCI);
		#else
		m_threadSupportCollision = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
			"collision",
			processCollisionTask,
			createCollisionLocalStoreMemory,
			maxNumOutstandingTasks));
		#endif

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration. 
		btDefaultCollisionConstructionInfo cci;
		cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
		collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration(cci);


		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		//dispatcher = new btCollisionDispatcher(collisionConfiguration);
		dispatcher = new SpuGatheringCollisionDispatcher(m_threadSupportCollision, maxNumOutstandingTasks, collisionConfiguration);
		softBodyWorldInfo.m_dispatcher = dispatcher;

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep. 
		overlappingPairCache = new btDbvtBroadphase(); 
		softBodyWorldInfo.m_broadphase = overlappingPairCache;
		
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded) 
		//solver = new btSequentialImpulseConstraintSolver;
		#ifdef SEQUENTIAL
		solver = new btSequentialImpulseConstraintSolver();
		#else
		m_threadSupportSolver = createSolverThreadSupport(maxNumOutstandingTasks);
		solver = new btParallelConstraintSolver(m_threadSupportSolver);

		//this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
		dispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);
		#endif

		dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration); // , softBodySolver);

		dynamicsWorld->getSimulationIslandManager()->setSplitIslands(false);
		dynamicsWorld->getSolverInfo().m_numIterations = 4;
		dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_SIMD + SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

		dynamicsWorld->getDispatchInfo().m_enableSPU = true;

		dynamicsWorld->setGravity(btVector3(0,-9.8f,0));

		softBodyWorldInfo.m_gravity.setValue(0, -9.8f, 0);
		softBodyWorldInfo.m_sparsesdf.Initialize();
	}


	DynamicsWorld::~DynamicsWorld()
	{
		//remove the rigidbodies from the dynamics world and delete them 
		for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ; i--) 
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i]; 
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{ 
				delete body->getMotionState(); 
			} 
			dynamicsWorld->removeCollisionObject( obj ); 
			delete obj;
		}

		//delete collision shapes
		for (int j = 0; j < collisionShapes.size(); j++)
		{ 
			btCollisionShape* shape = collisionShapes[j]; 
			collisionShapes[j] = 0; 
			delete shape; 
		} 

		delete dynamicsWorld;
		delete solver;

		if (m_threadSupportSolver)
		{
			delete m_threadSupportSolver;
		}

		delete overlappingPairCache;
		delete dispatcher;

		deleteCollisionLocalStoreMemory();
		if (m_threadSupportCollision)
		{
			delete m_threadSupportCollision;
		}

		delete collisionConfiguration;
	}


	void DynamicsWorld::AddBody(RigidBody* body)
	{
		RigidBodies.push_back(body);

		dynamicsWorld->addRigidBody(body->GetBody());
		collisionShapes.push_back(body->GetBody()->getCollisionShape());
		BodyMap[body->GetBody()] = body;
	}


	void DynamicsWorld::AddBody(SoftBody* psb)
	{
		SoftBodies.push_back(psb);
		dynamicsWorld->addSoftBody(psb->GetBody());
		BodyMap[psb->GetBody()] = psb;
	}


	void DynamicsWorld::Update()
	{
		dynamicsWorld->stepSimulation(Time::Delta, 10);

		for (auto fb : RigidBodies)
		{
			btRigidBody* body = fb->GetBody();
			
			if (body && body->getMotionState())
			{
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);

				auto p = trans.getOrigin();
				auto r = trans.getRotation();
				fb->Entity->Transform.Position = glm::vec3(p.getX(), p.getY(), p.getZ());
				fb->Entity->Transform.Rotation = glm::quat(r.getW(), r.getX(), r.getY(), r.getZ());
			}
		}

		for (auto sb : SoftBodies)
		{
			sb->RebuildMesh();
		}

		softBodyWorldInfo.m_sparsesdf.GarbageCollect();
	}


	struct BroadphaseCallback : public btBroadphaseAabbCallback {
		btCollisionObject *m_collisionObject;
		std::vector<Entity*> *m_entities;
		short int m_collisionFilterGroup, m_collisionFilterMask;
		std::unordered_map<btCollisionObject*, FreeBody*> *BodyMap;

		BroadphaseCallback(std::unordered_map<btCollisionObject*, FreeBody*> &BodyMap, btCollisionObject *me,
			std::vector<Entity*> &entities, short int collisionGroup, short int collisionMask) :
			BodyMap(&BodyMap), m_collisionObject(me), m_entities(&entities), m_collisionFilterGroup(collisionGroup), m_collisionFilterMask(collisionMask) { }

		inline bool needsCollision(btBroadphaseProxy* proxy0) const {
			bool collides = (proxy0->m_collisionFilterGroup & m_collisionFilterMask) != 0;
			collides = collides && (m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}

		virtual bool process(const btBroadphaseProxy *proxy) {
			btCollisionObject *collisionObject = (btCollisionObject *)proxy->m_clientObject;
			
			// don't add self to results
			if (collisionObject == m_collisionObject)
				return true;

			// check collision filters and masks
			if (needsCollision(collisionObject->getBroadphaseHandle())) {
				
				m_entities->push_back((*BodyMap)[collisionObject]->Entity);

			}
			return true;
		}
	};

	void DynamicsWorld::GetAllEntitiesWithinBroadphase(btCollisionObject& shape, std::vector<Entity*> &entities)
	{
		// Get a list of entities that could be intersecting with the shape object using bullet's broadphase test
		btVector3 min, max;
		shape.getCollisionShape()->getAabb(shape.getWorldTransform(), min, max);
		BroadphaseCallback callback(BodyMap, &shape, entities, 0, 0);
		dynamicsWorld->getBroadphase()->aabbTest(min, max, callback);
	}

}