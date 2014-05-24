#pragma once
#include "Core.h"
#include "FreeBody.h"

namespace Core
{
	class DynamicsWorld;
	class Material;
	
	class RigidBody
		: public FreeBody
	{		
	public:
		enum class Type { DYNAMIC, KINEMATIC, STATIC };

		RigidBody(DynamicsWorld* world, Material* material, Core::Shape* shape, Type type = Type::DYNAMIC);
		virtual ~RigidBody();
		virtual void Load();

		btRigidBody* GetBody();
		
		float GetMass();
		void SetGravity(const glm::vec3& g);		
		void ApplyForce(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterForce(glm::vec3 force);
		void ApplyImpulse(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterImpulse(glm::vec3 force);
		void ApplyTorque(const glm::vec3& torque);
		void ApplyTorqueImpulse(const glm::vec3& torque);

	protected:
		Type type;
		Shape* Shape;

		btRigidBody* body;
	};

}