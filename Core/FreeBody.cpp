#include "FreeBody.h"

namespace Core
{

	FreeBody::FreeBody(DynamicsWorld* world, float mass)
	{
		World = world;
		Mass = mass;

		Shape = nullptr;
		Material = nullptr;

		world->AddBody(this);
	}


	FreeBody::~FreeBody() 
	{
		if (Shape != nullptr)
			delete Shape;
	}


	void FreeBody::Load()
	{
		LastPosition = Entity->Transform.Position;
		NextPosition = Entity->Transform.Position;
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
		Impulse += force;
	}


	glm::vec3 FreeBody::CalculateTotalImpulse(glm::vec3 ExternalForces, float TimeStep)
	{
		glm::vec3 value = Impulse;
		value += ExternalForces * TimeStep;
		for (auto p : Forces)
		{
			value += p.second * TimeStep;
		}
		Impulse = glm::vec3(0.0f, 0.0f, 0.0f);
		return value;
	}

	
	float FreeBody::GetMass()
	{
		return Mass;
	}

}