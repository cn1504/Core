#pragma once
#include "Core.h"
#include "FreeBody.h"
#include "Kernel.h"
#include <atomic>

#define MAX_BODIES (1000)

namespace Core
{
	class Window;
	class FreeBody;

	class DynamicsWorld
	{
	private:
		Window* Window;
		Kernel* ForwardIntegrationKernel;

		std::vector<FreeBody*> FreeBodies;
		std::atomic<int> BodyCount;

		// Freebody object buffers
		float sumForces[MAX_BODIES * 3];
		float sumTorques[MAX_BODIES * 3];
		float accGravity[MAX_BODIES * 3];
		float invMass[MAX_BODIES];
		float invInertia[MAX_BODIES * 3];

		float Velocity[MAX_BODIES * 3];
		float AngularVelocity[MAX_BODIES * 3];

		float LastPosition[MAX_BODIES * 3];
		float NextPosition[MAX_BODIES * 3];

		float LastRotation[MAX_BODIES * 4];
		float NextRotation[MAX_BODIES * 4];

		void GPUPhysics();

		float TimeStep;
		float TimeSinceUpdate;

	public:
		DynamicsWorld(Core::Window* window, float timestep);
		~DynamicsWorld();

		int AddBody(FreeBody* body);

		void Update();
		void StartSimulation();
	};

}