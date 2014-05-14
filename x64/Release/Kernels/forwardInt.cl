__kernel void forwardInt(__global const float * forceSum,
	__global const float * torqueSum,
	__global const float * gravity,
	__global const float * invMass,
	__global const float * invInertia,
	__global float * velocity,
	__global float * angularVelocity,
	__global float * lastPosition,
	__global float * nextPosition,
	__global float * lastRotation,
	__global float * nextRotation,
	const float timestep,
	const int num)
{
	const int idx = get_global_id(0);
	
	if (idx < num)
	{		
		int idx3x = idx * 3;
		int idx3y = idx * 3 + 1;
		int idx3z = idx * 3 + 2;
		int idx4x = idx * 4;
		int idx4y = idx * 4 + 1;
		int idx4z = idx * 4 + 2;
		int idx4w = idx * 4 + 3;
		
		velocity[idx3x] += (forceSum[idx3x] * invMass[idx] + gravity[idx3x]) * timestep;
		velocity[idx3y] += (forceSum[idx3y] * invMass[idx] + gravity[idx3y]) * timestep;
		velocity[idx3z] += (forceSum[idx3z] * invMass[idx] + gravity[idx3z]) * timestep;
		angularVelocity[idx3x] += torqueSum[idx3x] * invInertia[idx3x] * timestep;
		angularVelocity[idx3y] += torqueSum[idx3y] * invInertia[idx3y] * timestep;
		angularVelocity[idx3z] += torqueSum[idx3z] * invInertia[idx3z] * timestep;

		lastPosition[idx3x] = nextPosition[idx3x];
		lastPosition[idx3y] = nextPosition[idx3y];
		lastPosition[idx3z] = nextPosition[idx3z];
		nextPosition[idx3x] += velocity[idx3x] * timestep;
		nextPosition[idx3y] += velocity[idx3y] * timestep;
		nextPosition[idx3z] += velocity[idx3z] * timestep;
		
		// Update rotation quat
		lastRotation[idx4x] = nextRotation[idx4x];
		lastRotation[idx4y] = nextRotation[idx4y];
		lastRotation[idx4z] = nextRotation[idx4z];
		lastRotation[idx4w] = nextRotation[idx4w];
		float4 q;
			q.x = lastRotation[idx4x];
			q.y = lastRotation[idx4y];
			q.z = lastRotation[idx4z];
			q.w = lastRotation[idx4w];
		
		// Convert eulerangle velocity to a quaternion rotation
		float3 eulerAngle;
		eulerAngle.x = angularVelocity[idx3x] * timestep;
		eulerAngle.y = angularVelocity[idx3y] * timestep;
		eulerAngle.z = angularVelocity[idx3z] * timestep;
		
		float3 c = cos(eulerAngle * 0.5);
		float3 s = sin(eulerAngle * 0.5);
		
		float4 p;
			p.w = c.x * c.y * c.z + s.x * s.y * s.z;
			p.x = s.x * c.y * c.z - c.x * s.y * s.z;
			p.y = c.x * s.y * c.z + s.x * c.y * s.z;
			p.z = c.x * c.y * s.z - s.x * s.y * c.z;
		
		// Quat * Quat
		nextRotation[idx4w] = q.w * p.w - q.x * p.x - q.y * p.y - q.z * p.z;
		nextRotation[idx4x] = q.w * p.x + q.x * p.w + q.y * p.z - q.z * p.y;
		nextRotation[idx4y] = q.w * p.y + q.y * p.w + q.z * p.x - q.x * p.z;
		nextRotation[idx4z] = q.w * p.z + q.z * p.w + q.x * p.y - q.y * p.x;
	}
}