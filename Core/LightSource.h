#pragma once
#include "Core.h"
#include "Component.h"
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace Core
{
	class Scene;
	class Texture;
	class Entity;

	class LightSource
		: public Component
	{
	protected:
		Scene* scene;
		bool shadow;
		btSphereShape* shape;
		std::vector<Core::Entity*> litObjects;

	public:
		Texture* ShadowMap;
		glm::mat4 Projection;
		glm::vec3 Color;

		LightSource(Scene* scene, glm::vec3 color, bool castsShadow = false);
		virtual ~LightSource();

		virtual void Load();
		virtual void Update();

		bool CastsShadow();
		std::vector<Core::Entity*>* GetLitObjects();
	};

}