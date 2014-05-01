#version 130

uniform vec3 SpherePosition;
uniform float Radius;
uniform vec4 DiffuseColor;

in vec3 viewVertex;
in vec3 viewNormal;

void main(void)
{	
	vec3 rayDirection = normalize(viewVertex);
	
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
	
	 gl_FragData[1] = vec4(normalize(cameraPos - SpherePosition), 0.0);
	
	 gl_FragData[0] = DiffuseColor;
}