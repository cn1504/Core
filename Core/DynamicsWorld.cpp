#include "DynamicsWorld.h"
#include "Window.h"

namespace Core
{

	DynamicsWorld::DynamicsWorld(Core::Window* window, float timestep)
	{
		Window = window;

		ForwardIntegrationKernel = new Kernel(Window->clContext, Window->clDevices, "Kernels/forwardInt.cl", "forwardInt");
		size_t size = sizeof(float)* MAX_BODIES;

		// Initialize buffers
		sumForcesBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_ONLY, size * 3);
		sumTorquesBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_ONLY, size * 3);
		accGravityBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_ONLY, size * 3);
		invMassBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_ONLY, size);
		invInertiaBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_ONLY, size * 3);

		VelocityBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_WRITE, size * 3);
		AngularVelocityBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_WRITE, size * 3);

		LastPositionBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_WRITE, size * 3);
		NextPositionBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_WRITE, size * 3);
		LastRotationBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_WRITE, size * 4);
		NextRotationBuffer = new cl::Buffer(*Window->clContext, CL_MEM_READ_WRITE, size * 4);
				

		TimeStep = timestep;
		TimeSinceUpdate = 0.0f;

		BodyCount = 0;
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
		size_t size = sizeof(float)* MAX_BODIES;
		
		// Write to buffers
		err = Window->clQueue->enqueueWriteBuffer(*sumForcesBuffer, CL_TRUE, 0, size * 3, sumForces);
		err = Window->clQueue->enqueueWriteBuffer(*sumTorquesBuffer, CL_TRUE, 0, size * 3, sumTorques);
		err = Window->clQueue->enqueueWriteBuffer(*invMassBuffer, CL_TRUE, 0, size, invMass);
		err = Window->clQueue->enqueueWriteBuffer(*invInertiaBuffer, CL_TRUE, 0, size * 3, invInertia);
		err = Window->clQueue->enqueueWriteBuffer(*accGravityBuffer, CL_TRUE, 0, size * 3, accGravity);
		err = Window->clQueue->enqueueWriteBuffer(*VelocityBuffer, CL_TRUE, 0, size * 3, Velocity);
		err = Window->clQueue->enqueueWriteBuffer(*AngularVelocityBuffer, CL_TRUE, 0, size * 3, AngularVelocity);
		err = Window->clQueue->enqueueWriteBuffer(*LastPositionBuffer, CL_TRUE, 0, size * 3, LastPosition);
		err = Window->clQueue->enqueueWriteBuffer(*NextPositionBuffer, CL_TRUE, 0, size * 3, NextPosition);
		err = Window->clQueue->enqueueWriteBuffer(*LastRotationBuffer, CL_TRUE, 0, size * 4, LastRotation);
		err = Window->clQueue->enqueueWriteBuffer(*NextRotationBuffer, CL_TRUE, 0, size * 4, NextRotation);
		Debug::CLError(err, "Unable to write to OpenCL buffers.");

		// Set kernel arguments
		err = ForwardIntegrationKernel->SetArg(0, *sumForcesBuffer);
		err |= ForwardIntegrationKernel->SetArg(1, *sumTorquesBuffer);
		err |= ForwardIntegrationKernel->SetArg(2, *accGravityBuffer);
		err |= ForwardIntegrationKernel->SetArg(3, *invMassBuffer);
		err |= ForwardIntegrationKernel->SetArg(4, *invInertiaBuffer);
		err |= ForwardIntegrationKernel->SetArg(5, *VelocityBuffer);
		err |= ForwardIntegrationKernel->SetArg(6, *AngularVelocityBuffer);
		err |= ForwardIntegrationKernel->SetArg(7, *LastPositionBuffer);
		err |= ForwardIntegrationKernel->SetArg(8, *NextPositionBuffer);
		err |= ForwardIntegrationKernel->SetArg(9, *LastRotationBuffer);
		err |= ForwardIntegrationKernel->SetArg(10, *NextRotationBuffer);
		err |= ForwardIntegrationKernel->SetArg(11, TimeStep);
		err |= ForwardIntegrationKernel->SetArg(12, BodyCount + 0);
		Debug::CLError(err, "Unable to set OpenCL kernel arguments.");

		// Execute Kernel
		cl::NDRange localSize(64);
		cl::NDRange globalSize((int)(ceil(BodyCount / (double)64) * 64));

		cl::Event event;
		err = Window->clQueue->enqueueNDRangeKernel(
			*ForwardIntegrationKernel->GetID(),
			cl::NullRange,
			globalSize,
			localSize,
			NULL,
			&event);
		Debug::CLError(err, "Unable to queue OpenCL kernel.");

		event.wait();	// Block until event complete

		// Read result
		err = Window->clQueue->enqueueReadBuffer(*VelocityBuffer, CL_TRUE, 0, size * 3, Velocity);
		err = Window->clQueue->enqueueReadBuffer(*AngularVelocityBuffer, CL_TRUE, 0, size * 3, AngularVelocity);
		err = Window->clQueue->enqueueReadBuffer(*LastPositionBuffer, CL_TRUE, 0, size * 3, LastPosition);
		err = Window->clQueue->enqueueReadBuffer(*NextPositionBuffer, CL_TRUE, 0, size * 3, NextPosition);
		err = Window->clQueue->enqueueReadBuffer(*LastRotationBuffer, CL_TRUE, 0, size * 4, LastRotation);
		err = Window->clQueue->enqueueReadBuffer(*NextRotationBuffer, CL_TRUE, 0, size * 4, NextRotation);
		Debug::CLError(err, "Unable to read completed OpenCL kernel buffers.");
	}

}