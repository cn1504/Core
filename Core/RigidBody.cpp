#include "RigidBody.h"
#include "DynamicsWorld.h"
#include "Material.h"

namespace Core
{

	RigidBody::RigidBody(DynamicsWorld* world, Core::Material* material, Core::Shape* shape, Type type)
		: FreeBody(world, material)
	{
		this->type = type;
		Shape = shape;
	}


	RigidBody::~RigidBody()
	{
		if (Shape != nullptr)
			delete Shape;
	}


	void RigidBody::Load()
	{
		btCollisionShape* colShape = Shape->GetCollisionShape(Entity->Transform.Scale);

		btTransform startTransform;
		startTransform.setIdentity();

		startTransform.setOrigin(btVector3(Entity->Transform.Position.x, Entity->Transform.Position.y, Entity->Transform.Position.z));
		startTransform.setRotation(btQuaternion(Entity->Transform.Rotation.x, Entity->Transform.Rotation.y, Entity->Transform.Rotation.z, Entity->Transform.Rotation.w));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btScalar mass(0.0);
		btVector3 localInertia(0, 0, 0);
		if (type == Type::DYNAMIC)
		{
			float volume = Shape->CalculateVolume(Entity->Transform.Scale);
			mass = btScalar(material->Density * volume);
			colShape->calculateLocalInertia(mass, localInertia);
		}
		else // Static or Kinematic
		{
			mass = btScalar(0.0);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		body = new btRigidBody(rbInfo);

		if (type == Type::KINEMATIC)
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			body->setActivationState(DISABLE_DEACTIVATION);
		}

		world->AddBody(this);
	}


	btRigidBody* RigidBody::GetBody()
	{
		return body;
	}


	float RigidBody::GetMass()
	{
		return 1.0f / body->getInvMass();
	}


	glm::vec3 RigidBody::GetVelocity()
	{
		auto v = body->getLinearVelocity();
		return glm::vec3(v.getX(), v.getY(), v.getZ());
	}


	void RigidBody::SetGravity(const glm::vec3& g)
	{
		body->setGravity(btVector3(g.x, g.y, g.z));
	}


	void RigidBody::ApplyForce(const glm::vec3& force, const glm::vec3& location)
	{
		body->applyForce(btVector3(force.x, force.y, force.z), btVector3(location.x, location.y, location.z));
	}
	void RigidBody::ApplyCenterForce(glm::vec3 force)
	{
		body->applyCentralForce(btVector3(force.x, force.y, force.z));
	}

	void RigidBody::ApplyImpulse(const glm::vec3& force, const glm::vec3& location)
	{
		body->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(location.x, location.y, location.z));
	}
	void RigidBody::ApplyCenterImpulse(glm::vec3 force)
	{
		body->applyCentralImpulse(btVector3(force.x, force.y, force.z));
	}


	void RigidBody::ApplyTorque(const glm::vec3& torque)
	{
		body->applyTorque(btVector3(torque.x, torque.y, torque.z));
	}
	void RigidBody::ApplyTorqueImpulse(const glm::vec3& torque)
	{
		body->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
	}

}