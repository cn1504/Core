#pragma once
#include "Core.h"
#include "FreeBody.h"

namespace Core
{
	class FreeBody;

	class DynamicsWorld
	{
	private:
		std::vector<FreeBody*> FreeBodies;
		float TimeStep;
		float TimeSinceUpdate;

	public:
		glm::vec3 Gravity;

		DynamicsWorld(float timestep);
		~DynamicsWorld();

		void AddBody(FreeBody* body);

		void Update();
		void StartSimulation();
	};

}