#include "DynamicsWorld.h"

namespace Core
{

	DynamicsWorld::DynamicsWorld(float timestep)
	{
		TimeStep = timestep;
		TimeSinceUpdate = 0.0f;
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
		while (TimeSinceUpdate > TimeStep)
		{
			for (auto b : FreeBodies)
			{
				b->SetGravity(Gravity);
				b->IntegrateForward(TimeStep);
			}

			TimeSinceUpdate -= TimeStep;
		}
		
		float lerp = TimeSinceUpdate / TimeStep;
		for (auto b : FreeBodies)
		{
			b->Interpolate(lerp);
		}
	}

}