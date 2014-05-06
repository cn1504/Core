#include "DynamicsWorld.h"

namespace Core
{

	DynamicsWorld::DynamicsWorld(float timestep)
	{
		TimeStep = timestep;
	}

	DynamicsWorld::~DynamicsWorld()
	{
	}

	void DynamicsWorld::AddBody(FreeBody* body)
	{
		FreeBodies.push_back(body);
	}

	void DynamicsWorld::Update()
	{
		TimeSinceUpdate += Time::Delta;
		if (TimeSinceUpdate > TimeStep)
		{
			for (auto b : FreeBodies)
			{
				glm::vec3 impulse = b->CalculateTotalImpulse(glm::vec3(0.0f,0.0f,0.0f), TimeStep);
				glm::vec3 a = impulse / b->GetMass() + Gravity;
				b->Velocity += a * TimeStep;
				b->LastPosition = b->NextPosition;
				b->NextPosition += b->Velocity * TimeStep;
			}

			TimeSinceUpdate -= TimeStep;
		}
		
		float lerp = TimeSinceUpdate / TimeStep;
		for (auto b : FreeBodies)
		{
			b->Entity->Transform.Position = lerp * b->NextPosition + (1.0f - lerp) * b->LastPosition;
		}
	}

}