#include "DynamicsWorld.h"
#include "Window.h"


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
		int maxNumOutstandingTasks = 4;

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
		collisionConfiguration = new btDefaultCollisionConfiguration(cci);


		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		//dispatcher = new btCollisionDispatcher(collisionConfiguration);
		dispatcher = new SpuGatheringCollisionDispatcher(m_threadSupportCollision, maxNumOutstandingTasks, collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep. 
		overlappingPairCache = new btDbvtBroadphase(); 
		
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

		btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		dynamicsWorld = world;

		world->getSimulationIslandManager()->setSplitIslands(false);
		world->getSolverInfo().m_numIterations = 4;
		world->getSolverInfo().m_solverMode = SOLVER_SIMD + SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

		dynamicsWorld->getDispatchInfo().m_enableSPU = true;

		dynamicsWorld->setGravity(btVector3(0,-10,0));
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

	void DynamicsWorld::AddBody(FreeBody* body)
	{
		FreeBodies.push_back(body);

		dynamicsWorld->addRigidBody(body->GetBody());
		collisionShapes.push_back(body->GetBody()->getCollisionShape());
	}

	void DynamicsWorld::Update()
	{
		dynamicsWorld->stepSimulation(Time::Delta, 10);

		for (auto fb : FreeBodies)
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
	}

}