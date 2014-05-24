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

		std::unordered_map<btCollisionObject*, FreeBody*> BodyMap;

	public:
		btSoftBodyWorldInfo	softBodyWorldInfo;

		DynamicsWorld();
		~DynamicsWorld();

		void AddBody(RigidBody* body);
		void AddBody(SoftBody* body);

		void Update();

		void GetAllEntitiesWithinBroadphase(btCollisionObject& shape, std::vector<Entity*> &entities);
	};

}