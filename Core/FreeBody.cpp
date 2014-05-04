#include "FreeBody.h"

namespace Core
{

	FreeBody::FreeBody(Core::Scene* scene, float mass)
	{
		Scene = scene;
		Mass = mass;

		Shape = nullptr;
		Material = nullptr;
	}


	FreeBody::~FreeBody() 
	{
	}


	void FreeBody::Load()
	{
		
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
			Mass = Material->Density * volume;
		}
	}


	void FreeBody::AddForce(std::string id, glm::vec3 force)
	{
		Forces[id] = force;
	}


	void FreeBody::AddImpulse(glm::vec3 force)
	{
		
	}

}