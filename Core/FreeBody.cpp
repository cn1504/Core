#include "FreeBody.h"

namespace Core
{

	FreeBody::FreeBody(DynamicsWorld* world, Core::Shape* shape, Core::Material* material, bool isDynamic)
	{
		World = world;

		Shape = shape;
		Material = material;
		IsDynamic = isDynamic;
	}


	FreeBody::~FreeBody() 
	{
		if (Shape != nullptr)
			delete Shape;
	}


	void FreeBody::Load()
	{
		colShape = Shape->GetCollisionShape(Entity->Transform.Scale);

		btScalar mass(0.0);
		btVector3 localInertia(0, 0, 0);
		if (IsDynamic)
		{
			float volume = Shape->CalculateVolume(Entity->Transform.Scale);
			mass = btScalar(Material->Density * volume);
			colShape->calculateLocalInertia(mass, localInertia);
		}
		else
		{
			mass = btScalar(0.0);
		}

		btTransform startTransform;
		startTransform.setIdentity();

		startTransform.setOrigin(btVector3(Entity->Transform.Position.x, Entity->Transform.Position.y, Entity->Transform.Position.z));
		startTransform.setRotation(btQuaternion(Entity->Transform.Rotation.x, Entity->Transform.Rotation.y, Entity->Transform.Rotation.z, Entity->Transform.Rotation.w));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		body = new btRigidBody(rbInfo);

		World->AddBody(this);
	}


	void FreeBody::Update()
	{
	}


	btRigidBody* FreeBody::GetBody()
	{
		return body;
	}


	float FreeBody::GetMass()
	{
		return 1.0f / body->getInvMass();
	}


	void FreeBody::SetGravity(const glm::vec3& g)
	{
		body->setGravity(btVector3(g.x, g.y, g.z));
	}


	void FreeBody::ApplyForce(const glm::vec3& force, const glm::vec3& location)
	{
		body->applyForce(btVector3(force.x, force.y, force.z), btVector3(location.x, location.y, location.z));
	}
	void FreeBody::ApplyCenterForce(glm::vec3 force)
	{
		body->applyCentralForce(btVector3(force.x, force.y, force.z));
	}

	void FreeBody::ApplyImpulse(const glm::vec3& force, const glm::vec3& location)
	{
		body->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(location.x, location.y, location.z));
	}
	void FreeBody::ApplyCenterImpulse(glm::vec3 force)
	{
		body->applyCentralImpulse(btVector3(force.x, force.y, force.z));
	}


	void FreeBody::ApplyTorque(const glm::vec3& torque)
	{
		body->applyTorque(btVector3(torque.x, torque.y, torque.z));
	}
	void FreeBody::ApplyTorqueImpulse(const glm::vec3& torque)
	{
		body->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
	}

}