#version 330

uniform mat4 ProjectionMatrix;
uniform vec3 SpherePosition;
uniform float Radius;
uniform vec4 DiffuseColor;

layout(location=0) in vec3 Vertex;
layout(location=1) in vec3 Normal;

layout(location=0) out vec4 diffuseOut;
layout(location=1) out vec4 normalOut;

void main(void)
{	
	vec3 rayDirection = normalize(Vertex);
	
	float B = 2.0 * dot(rayDirection, -SpherePosition);
	float C = dot(SpherePosition, SpherePosition) - (Radius * Radius);
	
	float det = (B * B) - (4 * C);
	if(det < 0.0)
		discard;
		
	float sqrtDet = sqrt(det);
	float posT = (-B + sqrtDet)/2;
	float negT = (-B - sqrtDet)/2;
	
	float intersectT = min(posT, negT);
	vec3 cameraPos = rayDirection * intersectT;
	normalOut = vec4(normalize(cameraPos - SpherePosition), 0.0);
	
	diffuseOut = DiffuseColor;
}