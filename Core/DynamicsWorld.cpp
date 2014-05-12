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
		// Predict next body positions
		TimeSinceUpdate += Time::Delta;
		while (TimeSinceUpdate > TimeStep)
		{
			TimeSinceUpdate -= TimeStep;

			for (auto b : FreeBodies)
			{
				b->SetGravity(Gravity);
				b->IntegrateForward(TimeStep);
			}
			

			// Detect collisions
			for (auto b : FreeBodies)
			{
			}
		}
		
		
		// Interpolate current position for this frame
		float lerp = TimeSinceUpdate / TimeStep;
		for (auto b : FreeBodies)
		{
			b->Interpolate(lerp);
		}
	}

}