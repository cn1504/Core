#include "Core.h"
#include "Component.h"
#include "Shape.h"
#include "Scene.h"
#include "Material.h"

namespace Core
{
	
	class FreeBody
		: public Component
	{
	private:
		Scene* Scene;
		Shape* Shape;
		Material* Material;

		std::unordered_map<std::string, glm::vec3> Forces;
		float Mass;

	public:
		FreeBody(Core::Scene* scene, float mass = 0.0f);
		virtual ~FreeBody();
		virtual void Load();
		virtual void Update();

		void SetCollisionShape(Core::Shape* shape);
		void SetMaterial(Core::Material* material);

		void CalculateMass();

		void AddForce(std::string id, glm::vec3 force);
		void AddImpulse(glm::vec3 force);
	};

}