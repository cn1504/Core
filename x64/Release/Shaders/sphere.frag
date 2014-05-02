#version 130

uniform vec3 SpherePosition;
uniform float Radius;
uniform vec4 DiffuseColor;

in vec3 viewVertex;
in vec3 viewNormal;

void main(void)
{	
	float r = Radius;	
	vec3 q0 = vec3(0.0, 0.0, 0.0);
	vec3 v = normalize(viewVertex);
	vec3 p = SpherePosition;
	
	float sc = dot(v, p);
	vec3 qc = q0 + sc*v;
	vec3 wc = qc - p;
	float d = length(wc);
	if(d > r)
		discard;
	
	vec3 qi = q0 + (sc - sqrt(r*r - d*d)) * v;
	
	gl_FragData[1] = vec4((normalize(qi - p) + 1) * 0.5, 0.0);
	gl_FragData[0] = DiffuseColor;
}