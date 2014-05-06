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

		std::unordered_map<std::string, glm::vec3> Forces;
		std::unordered_map<std::string, glm::vec3> Torques;
		glm::vec3 Impulse;
		float Mass;
		
	public:
		FreeBody(DynamicsWorld* world, float mass = 0.0f);
		virtual ~FreeBody();
		virtual void Load();
		virtual void Update();

		void SetCollisionShape(Core::Shape* shape);
		void SetMaterial(Core::Material* material);

		void CalculateMass();
		float GetMass();

		void AddForce(std::string id, glm::vec3 force);
		void AddImpulse(glm::vec3 force);

		glm::vec3 CalculateTotalImpulse(glm::vec3 ExternalForces, float TimeStep);

		glm::vec3 Velocity;	// ReadOnly
		glm::vec3 LastPosition;
		glm::vec3 NextPosition;
	};

}