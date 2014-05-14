__kernel void forwardInt(__global const float3 * forceSum,
	__global const float3 * torqueSum,
	__global const float3 * gravity,
	__global const float * invMass,
	__global const float3 * invInertia,
	__global float3 * velocity,
	__global float3 * angularVelocity,
	__global float3 * lastPosition,
	__global float3 * nextPosition,
	__global float4 * lastRotation,
	__global float4 * nextRotation,
	const float timestep,
	const int num)
{
	const int idx = get_global_id(0);
	
	if (idx < num)
	{		
		velocity[idx] = velocity[idx] + (forceSum[idx] * invMass[idx] + gravity[idx]) * timestep;
		angularVelocity[idx] = angularVelocity[idx] + torqueSum[idx] * invInertia[idx] * timestep;
		
		lastPosition[idx] = nextPosition[idx];
		nextPosition[idx] = nextPosition[idx] + velocity[idx] * timestep;
		
		// Update rotation quat
		float4 q = lastRotation[idx] = nextRotation[idx];
		
		// Convert eulerangle velocity to a quaternion rotation
		float3 eulerAngle = angularVelocity[idx] * timestep;
		float3 c = cos(eulerAngle * 0.5);
		float3 s = sin(eulerAngle * 0.5);
		
		float4 p   = {s.x * c.y * c.z - c.x * s.y * s.z,
					  c.x * s.y * c.z + s.x * c.y * s.z,
					  c.x * c.y * s.z - s.x * s.y * c.z,
					  c.x * c.y * c.z + s.x * s.y * s.z};
		
		// Quat * Quat
		nextRotation[idx].w = q.w * p.w - q.x * p.x - q.y * p.y - q.z * p.z;
		nextRotation[idx].x = q.w * p.x + q.x * p.w + q.y * p.z - q.z * p.y;
		nextRotation[idx].y = q.w * p.y + q.y * p.w + q.z * p.x - q.x * p.z;
		nextRotation[idx].z = q.w * p.z + q.z * p.w + q.x * p.y - q.y * p.x;
	}
}