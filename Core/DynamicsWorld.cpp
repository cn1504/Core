#include "DynamicsWorld.h"
#include "Window.h"

namespace Core
{

	DynamicsWorld::DynamicsWorld(Core::Window* window, float timestep)
	{
		Window = window;
		ForwardIntegrationKernel = new Kernel(Window->clContext, Window->clDevices, "Kernels/forwardInt.cl", "forwardInt");

		TimeStep = timestep;
		TimeSinceUpdate = 0.0f;
	}

	DynamicsWorld::~DynamicsWorld()
	{
		delete ForwardIntegrationKernel;
	}

	int DynamicsWorld::AddBody(FreeBody* body)
	{
		int i = BodyCount++;
		FreeBodies.push_back(body);

		body->SetBufferPointers(
			sumForces + i * 3,
			sumTorques + i * 3,
			accGravity + i * 3,
			invMass + i,
			invInertia + i * 3,
			Velocity + i * 3,
			AngularVelocity + i * 3, 
			LastPosition + i * 3, 
			NextPosition + i * 3,
			LastRotation + i * 4,
			NextRotation + i * 4 
			);

		return i;
	}

	void DynamicsWorld::Update()
	{
		// Predict next body positions
		TimeSinceUpdate += Time::Delta;
		while (TimeSinceUpdate > TimeStep)
		{
			TimeSinceUpdate -= TimeStep;

			if (Settings::Game::GPUPhysics > 0)
			{
				GPUPhysics();
			}
			else 
			{
				for (auto b : FreeBodies)
				{
					b->IntegrateForward(TimeStep);
				}


				// Detect collisions
				for (auto b : FreeBodies)
				{
				}
			}
		}
		
		
		// Interpolate current position for this frame
		float lerp = TimeSinceUpdate / TimeStep;
		for (auto b : FreeBodies)
		{
			b->Interpolate(lerp);
		}
	}


	void DynamicsWorld::GPUPhysics()
	{
		cl_int err;
		size_t size = sizeof(float) * BodyCount;

		// Initialize buffers
		cl::Buffer sumForcesBuffer(*Window->clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * 3, sumForces, &err);
		cl::Buffer sumTorquesBuffer(*Window->clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * 3, sumTorques, &err);
		cl::Buffer accGravityBuffer(*Window->clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * 3, accGravity, &err);
		cl::Buffer invMassBuffer(*Window->clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size, invMass, &err);
		cl::Buffer invInertiaBuffer(*Window->clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * 3, invInertia, &err);

		cl::Buffer VelocityBuffer(*Window->clContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * 3, Velocity, &err);
		cl::Buffer AngularVelocityBuffer(*Window->clContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * 3, AngularVelocity, &err);

		cl::Buffer LastPositionBuffer(*Window->clContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * 3, LastPosition, &err);
		cl::Buffer NextPositionBuffer(*Window->clContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * 3, NextPosition, &err);

		cl::Buffer LastRotationBuffer(*Window->clContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * 4, LastRotation, &err);
		cl::Buffer NextRotationBuffer(*Window->clContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * 4, NextRotation, &err);
		Debug::CLError(err, "Unable to create OpenCL buffer objects.");

		// Set kernel arguments
		err = ForwardIntegrationKernel->SetArg(0, sumForcesBuffer);
		err |= ForwardIntegrationKernel->SetArg(1, sumTorquesBuffer);
		err |= ForwardIntegrationKernel->SetArg(2, accGravityBuffer);
		err |= ForwardIntegrationKernel->SetArg(3, invMassBuffer);
		err |= ForwardIntegrationKernel->SetArg(4, invInertiaBuffer);
		err |= ForwardIntegrationKernel->SetArg(5, VelocityBuffer);
		err |= ForwardIntegrationKernel->SetArg(6, AngularVelocityBuffer);
		err |= ForwardIntegrationKernel->SetArg(7, LastPositionBuffer);
		err |= ForwardIntegrationKernel->SetArg(8, NextPositionBuffer);
		err |= ForwardIntegrationKernel->SetArg(9, LastRotationBuffer);
		err |= ForwardIntegrationKernel->SetArg(10, NextRotationBuffer);
		err |= ForwardIntegrationKernel->SetArg(11, TimeStep);
		err |= ForwardIntegrationKernel->SetArg(12, BodyCount+0);
		Debug::CLError(err, "Unable to set OpenCL kernel arguments.");

		// Execute Kernel
		cl::Event event;
		err = Window->clQueue->enqueueNDRangeKernel(
			*ForwardIntegrationKernel->GetID(),
			cl::NullRange,
			cl::NDRange(BodyCount),
			cl::NDRange(1, 1),
			NULL,
			&event);
		Debug::CLError(err, "Unable to queue OpenCL kernel.");

		event.wait();	// Block until event complete

		// Read result
		err = Window->clQueue->enqueueReadBuffer(VelocityBuffer, CL_FALSE, 0, size * 3, Velocity);
		err = Window->clQueue->enqueueReadBuffer(AngularVelocityBuffer, CL_FALSE, 0, size * 3, AngularVelocity);
		err = Window->clQueue->enqueueReadBuffer(LastPositionBuffer, CL_FALSE, 0, size * 3, LastPosition);
		err = Window->clQueue->enqueueReadBuffer(NextPositionBuffer, CL_FALSE, 0, size * 3, NextPosition);
		err = Window->clQueue->enqueueReadBuffer(LastRotationBuffer, CL_FALSE, 0, size * 4, LastRotation);
		err = Window->clQueue->enqueueReadBuffer(NextRotationBuffer, CL_FALSE, 0, size * 4, NextRotation);
		Window->clQueue->finish();
		Debug::CLError(err, "Unable to read completed OpenCL kernel buffers.");
	}

}