#pragma once
#include "Core.h"
#include "FreeBody.h"

class btThreadSupportInterface;

namespace Core
{
	class Window;
	class FreeBody;

	class DynamicsWorld
	{
	private:		
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		
		btThreadSupportInterface*		m_threadSupportCollision;
		btThreadSupportInterface*		m_threadSupportSolver;

		std::vector<FreeBody*> FreeBodies;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;

	public:
		DynamicsWorld();
		~DynamicsWorld();

		void AddBody(FreeBody* body);

		void Update();
	};

}