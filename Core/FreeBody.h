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

		glm::vec3 sumForces;
		glm::vec3 sumTorques;
		glm::vec3 accGravity;
		float invMass;
		glm::vec3 invInertia;

		glm::vec3 Velocity;
		glm::vec3 AngularVelocity;

		glm::vec3 LastPosition;
		glm::vec3 NextPosition;

		glm::quat LastRotation;
		glm::quat NextRotation;
		
	public:
		FreeBody(DynamicsWorld* world, float mass = 0.0f);
		virtual ~FreeBody();
		virtual void Load();
		virtual void Update();

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