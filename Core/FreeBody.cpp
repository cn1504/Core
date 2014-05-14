#include "FreeBody.h"

namespace Core
{

	FreeBody::FreeBody(DynamicsWorld* world, float mass)
	{
		World = world;
		BufferPosition = world->AddBody(this);

		invMass[0] = 1.0f / mass;

		Shape = nullptr;
		Material = nullptr;
		
		// Initialize values
		sumForces[0] = 0.0f;
		sumForces[1] = 0.0f;
		sumForces[2] = 0.0f;
		sumTorques[0] = 0.0f;
		sumTorques[1] = 0.0f;
		sumTorques[2] = 0.0f;
		accGravity[0] = 0.0f;
		accGravity[1] = 0.0f;
		accGravity[2] = 0.0f;
		invInertia[0] = 0.0f;
		invInertia[0] = 0.0f;
		invInertia[0] = 0.0f;
		Velocity[0] = 0.0f;
		Velocity[1] = 0.0f;
		Velocity[2] = 0.0f;
		AngularVelocity[0] = 0.0f;
		AngularVelocity[1] = 0.0f;
		AngularVelocity[2] = 0.0f;
	}


	FreeBody::~FreeBody() 
	{
		if (Shape != nullptr)
			delete Shape;
	}


	void FreeBody::SetBufferPointers(float* a, float* b, float* c, float* d, float* e, float* f, float* g, float* h, float* i, float* j, float* k)
	{
		sumForces = a;
		sumTorques = b;
		accGravity = c;
		invMass = d;
		invInertia = e;

		Velocity = f;
		AngularVelocity = g;

		LastPosition = h;
		NextPosition = i;

		LastRotation = j;
		NextRotation = k;
	}


	void FreeBody::Load()
	{
		LastPosition[0] = Entity->Transform.Position.x;
		LastPosition[1] = Entity->Transform.Position.y;
		LastPosition[2] = Entity->Transform.Position.z;
		NextPosition[0] = Entity->Transform.Position.x;
		NextPosition[1] = Entity->Transform.Position.y;
		NextPosition[2] = Entity->Transform.Position.z;

		LastRotation[0] = Entity->Transform.Rotation.x;
		LastRotation[1] = Entity->Transform.Rotation.y;
		LastRotation[2] = Entity->Transform.Rotation.z;
		LastRotation[3] = Entity->Transform.Rotation.w;
		NextRotation[0] = Entity->Transform.Rotation.x;
		NextRotation[1] = Entity->Transform.Rotation.y;
		NextRotation[2] = Entity->Transform.Rotation.z;
		NextRotation[3] = Entity->Transform.Rotation.w;
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
			invMass[0] = 1.0f / Mass;
			glm::vec3 invI = 1.0f / (Shape->CalculateInertia(Entity->Transform.Scale) * Mass);
			invInertia[0] = invI.x;
			invInertia[1] = invI.y;
			invInertia[2] = invI.z;
		}
	}
	float FreeBody::GetMass()
	{
		return 1.0f / invMass[0];
	}


	void FreeBody::SetGravity(const glm::vec3& g)
	{
		accGravity[0] = g.x;
		accGravity[1] = g.y;
		accGravity[2] = g.z;
	}


	void FreeBody::IntegrateForward(float timestep)
	{
		Velocity[0] += (sumForces[0] * invMass[0] + accGravity[0]) * timestep;
		Velocity[1] += (sumForces[1] * invMass[0] + accGravity[1]) * timestep;
		Velocity[2] += (sumForces[2] * invMass[0] + accGravity[2]) * timestep;
		AngularVelocity[0] += sumTorques[0] * invInertia[0] * timestep;
		AngularVelocity[1] += sumTorques[1] * invInertia[1] * timestep;
		AngularVelocity[2] += sumTorques[2] * invInertia[2] * timestep;

		LastPosition[0] = NextPosition[0];
		LastPosition[1] = NextPosition[1];
		LastPosition[2] = NextPosition[2];
		NextPosition[0] += Velocity[0] * timestep;
		NextPosition[1] += Velocity[1] * timestep;
		NextPosition[2] += Velocity[2] * timestep;

		LastRotation[0] = NextRotation[0];
		LastRotation[1] = NextRotation[1];
		LastRotation[2] = NextRotation[2];
		LastRotation[3] = NextRotation[3];
		glm::quat nr = glm::quat(NextRotation[3], NextRotation[0], NextRotation[1], NextRotation[2]) * glm::quat(glm::vec3(AngularVelocity[0], AngularVelocity[1], AngularVelocity[2]) * timestep);
		NextRotation[0] = nr.x; 
		NextRotation[1] = nr.y; 
		NextRotation[2] = nr.z; 
		NextRotation[3] = nr.w;
	}

	void FreeBody::Interpolate(float lerp)
	{
		Entity->Transform.Position = lerp * glm::vec3(NextPosition[0], NextPosition[1], NextPosition[2]) + (1.0f - lerp) * glm::vec3(LastPosition[0], LastPosition[1], LastPosition[2]);

		Entity->Transform.Rotation = glm::mix(glm::quat(LastRotation[3], LastRotation[0], LastRotation[1], LastRotation[2]), glm::quat(NextRotation[3], NextRotation[0], NextRotation[1], NextRotation[2]), lerp);
	}


	void FreeBody::ApplyForce(const glm::vec3& force, const glm::vec3& location)
	{
		ApplyCenterForce(force);
		ApplyTorque(glm::cross(location, force));
	}
	void FreeBody::ApplyCenterForce(glm::vec3 force)
	{
		sumForces[0] += force.x;
		sumForces[1] += force.y;
		sumForces[2] += force.z;
	}

	void FreeBody::ApplyImpulse(const glm::vec3& force, const glm::vec3& location)
	{
		ApplyCenterImpulse(force);
		ApplyTorqueImpulse(glm::cross(location, force));
	}
	void FreeBody::ApplyCenterImpulse(glm::vec3 force)
	{
		Velocity[0] += force.x * invMass[0];
		Velocity[1] += force.y * invMass[0];
		Velocity[2] += force.z * invMass[0];
	}


	void FreeBody::ApplyTorque(const glm::vec3& torque)
	{
		sumTorques[0] += torque.x;
		sumTorques[1] += torque.y;
		sumTorques[2] += torque.z;
	}
	void FreeBody::ApplyTorqueImpulse(const glm::vec3& torque)
	{
		AngularVelocity[0] += invInertia[0] * torque.x;
		AngularVelocity[1] += invInertia[1] * torque.y;
		AngularVelocity[2] += invInertia[2] * torque.z;
	}

}