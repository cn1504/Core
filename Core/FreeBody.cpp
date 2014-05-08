#include "FreeBody.h"

namespace Core
{

	FreeBody::FreeBody(DynamicsWorld* world, float mass)
	{
		World = world;
		invMass = 1.0f / mass;

		Shape = nullptr;
		Material = nullptr;

		world->AddBody(this);
	}


	FreeBody::~FreeBody() 
	{
		if (Shape != nullptr)
			delete Shape;
	}


	void FreeBody::Load()
	{
		LastPosition = Entity->Transform.Position;
		NextPosition = Entity->Transform.Position;
	}


	void FreeBody::Update()
	{
	}


	void FreeBody::SetCollisionShape(Core::Shape* shape)
	{
		Shape = shape;
	}
	

	void FreeBody::SetMaterial(Core::Material* material)
	{
		Material = material;
	}

	
	void FreeBody::CalculateMass()
	{
		if (Shape != nullptr && Material != nullptr)
		{
			float volume = Shape->CalculateVolume(Entity->Transform.Scale);
			float Mass = (Material->Density * volume);
			invMass = 1.0f / Mass;
			invInertia = 1.0f / (Shape->CalculateInertia(Entity->Transform.Scale) * Mass);
		}
	}
	float FreeBody::GetMass()
	{
		return 1.0f / invMass;
	}


	void FreeBody::SetGravity(const glm::vec3& g)
	{
		accGravity = g;
	}


	void FreeBody::IntegrateForward(float timestep)
	{
		glm::vec3 a = sumForces * invMass + accGravity;
		Velocity += a * timestep;
		AngularVelocity += sumTorques * invInertia * timestep;

		LastPosition = NextPosition;
		NextPosition += Velocity * timestep;

		LastRotation = NextRotation;
		NextRotation = NextRotation * glm::quat(AngularVelocity * timestep);
	}

	void FreeBody::Interpolate(float lerp)
	{
		Entity->Transform.Position = lerp * NextPosition + (1.0f - lerp) * LastPosition;

		Entity->Transform.Rotation = glm::mix(LastRotation, NextRotation, lerp);
	}


	void FreeBody::ApplyForce(const glm::vec3& force, const glm::vec3& location)
	{
		ApplyCenterForce(force);
		ApplyTorque(glm::cross(location, force));
	}
	void FreeBody::ApplyCenterForce(glm::vec3 force)
	{
		sumForces += force;
	}

	void FreeBody::ApplyImpulse(const glm::vec3& force, const glm::vec3& location)
	{
		ApplyCenterImpulse(force);
		ApplyTorqueImpulse(glm::cross(location, force));
	}
	void FreeBody::ApplyCenterImpulse(glm::vec3 force)
	{
		Velocity += force * invMass;
	}


	void FreeBody::ApplyTorque(const glm::vec3& torque)
	{
		sumTorques += torque;
	}
	void FreeBody::ApplyTorqueImpulse(const glm::vec3& torque)
	{
		AngularVelocity += invInertia * torque;
	}

}