#pragma once
#include "Core.h"
#include "RigidBody.h"
#include "SoftBody.h"

class btThreadSupportInterface;

namespace Core
{
	class Window;
	class RigidBody;
	class SoftBody;

	class DynamicsWorld
	{
	private:		
		btSoftBodyRigidBodyCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btSoftRigidDynamicsWorld* dynamicsWorld;
		
		btThreadSupportInterface*		m_threadSupportCollision;
		btThreadSupportInterface*		m_threadSupportSolver;

		std::vector<RigidBody*> RigidBodies;
		std::vector<SoftBody*> SoftBodies;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;

	public:
		std::unordered_map<btCollisionObject*, FreeBody*> BodyMap;

		btSoftBodyWorldInfo	softBodyWorldInfo;

		DynamicsWorld();
		~DynamicsWorld();

		void AddBody(btCollisionObject* body);
		void AddBody(RigidBody* body);
		void AddBody(SoftBody* body);

		void Update();

		void GetAllEntitiesWithinBroadphase(btCollisionShape& shape, btTransform& t, std::vector<Entity*> &entities);
	};

}