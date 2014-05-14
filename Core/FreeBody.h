#pragma once
#include "Core.h"
#include "Component.h"
#include "Shape.h"
#include "DynamicsWorld.h"
#include "Material.h"

namespace Core
{
	class DynamicsWorld;
	
	class FreeBody
		: public Component
	{
	private:
		DynamicsWorld* World;
		Shape* Shape;
		Material* Material;

		int BufferPosition;

		float* sumForces;
		float* sumTorques;
		float* accGravity;
		float* invMass;
		float* invInertia;

		float* Velocity;
		float* AngularVelocity;

		float* LastPosition;
		float* NextPosition;

		float* LastRotation;
		float* NextRotation;
		
	public:
		FreeBody(DynamicsWorld* world, float mass = 0.0f);
		virtual ~FreeBody();
		virtual void Load();
		virtual void Update();

		void SetBufferPointers(float* a, float* b, float* c, float* d, float* e, float* f, float* g, float* h, float* i, float* j, float* k);

		void SetCollisionShape(Core::Shape* shape);
		void SetMaterial(Core::Material* material);

		void CalculateMass();
		float GetMass();
		void SetGravity(const glm::vec3& g);

		void IntegrateForward(float timestep);
		void Interpolate(float lerp);

		void ApplyForce(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterForce(glm::vec3 force);

		void ApplyImpulse(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterImpulse(glm::vec3 force);

		void ApplyTorque(const glm::vec3& torque);
		void ApplyTorqueImpulse(const glm::vec3& torque);
	};

}