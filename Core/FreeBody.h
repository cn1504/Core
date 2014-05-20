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
		int BufferPosition;

		DynamicsWorld* World;
		Shape* Shape;
		Material* Material;
		bool IsDynamic;
		
		btRigidBody* body;
		btCollisionShape* colShape;
		
	public:
		FreeBody(DynamicsWorld* world, Core::Shape* shape, Core::Material* material, bool isDynamic);
		virtual ~FreeBody();
		virtual void Load();
		virtual void Update();

		btRigidBody* GetBody();
		
		float GetMass();
		void SetGravity(const glm::vec3& g);		
		void ApplyForce(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterForce(glm::vec3 force);
		void ApplyImpulse(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterImpulse(glm::vec3 force);
		void ApplyTorque(const glm::vec3& torque);
		void ApplyTorqueImpulse(const glm::vec3& torque);
	};

}