#include "LightSource.h"
#include "Scene.h"
#include "Texture.h"

namespace Core
{

	LightSource::LightSource(Scene* scene, glm::vec3 color, bool castsShadow)
	{
		this->scene = scene;
		Color = color;
		shadow = castsShadow;
		ShadowMap = nullptr;
	}


	LightSource::~LightSource()
	{
	}


	void LightSource::Load()
	{
		if (shadow)
		{
			shape = new btSphereShape(Entity->Transform.Scale.x / 2);

			ShadowMap = new Texture();
			ShadowMap->CreateCubeMap(true, Settings::Video::ShadowResolution, Settings::Video::ShadowResolution);
		}

		scene->AddLight(this);
	}
		

	void LightSource::Update()
	{
		if (shadow)
		{
			litObjects.clear();
			
			btTransform trans;
			trans.setIdentity();
			trans.setOrigin(btVector3(Entity->Transform.Position.x, Entity->Transform.Position.y, Entity->Transform.Position.z));
			shape->setUnscaledRadius(Entity->Transform.Scale.x / 2);

			scene->PhysicsWorld->GetAllEntitiesWithinBroadphase(*shape, trans, litObjects);
		}
	}


	bool LightSource::CastsShadow()
	{
		return shadow;
	}


	std::vector<Core::Entity*>* LightSource::GetLitObjects()
	{
		return &litObjects;
	}

}