__kernel void forwardInt(__global const float * forceSum,
	__global const float * torqueSum,
	__global const float * gravity,
	__global const float * invMass,
	__global const float * invInertia,
	__global float * velocity,
	__global float * angularVelocity,
	__global float * nextPosition,
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
		
		float ts = timestep;
		float3 vForces = (float3)(forceSum[idx3x], forceSum[idx3y], forceSum[idx3z]);
		float3 vTorques = (float3)(torqueSum[idx3x], torqueSum[idx3y], torqueSum[idx3z]);
		float3 vGrav = (float3)(gravity[idx3x], gravity[idx3y], gravity[idx3z]);
		float fMass = invMass[idx];
		float3 vInertia = (float3)(invInertia[idx3x], invInertia[idx3y], invInertia[idx3z]);
		float3 vVelocity = (float3)(velocity[idx3x], velocity[idx3y], velocity[idx3z]);
		float3 vAngVelocity = (float3)(angularVelocity[idx3x], angularVelocity[idx3y], angularVelocity[idx3z]);
		
		float3 vNP = (float3)(nextPosition[idx3x], nextPosition[idx3y], nextPosition[idx3z]);
		float4 vNR = (float4)(nextRotation[idx4x], nextRotation[idx4y], nextRotation[idx4z], nextRotation[idx4w]);
		
		vVelocity += (vForces * fMass + vGrav) * ts;
		vAngVelocity += vTorques * vInertia * ts;

		velocity[idx3x] = vVelocity.x;
		velocity[idx3y] = vVelocity.y;
		velocity[idx3z] = vVelocity.z;
		angularVelocity[idx3x] = vAngVelocity.x;
		angularVelocity[idx3y] = vAngVelocity.y;
		angularVelocity[idx3z] = vAngVelocity.z;
		
		vNP += vVelocity * ts;
		nextPosition[idx3x] = vNP.x;
		nextPosition[idx3y] = vNP.y;
		nextPosition[idx3z] = vNP.z; 
		
		// Update rotation quat
		float4 q = vNR;
		
		// Convert eulerangle velocity to a quaternion rotation
		float3 eulerAngle = vAngVelocity * ts;
		
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