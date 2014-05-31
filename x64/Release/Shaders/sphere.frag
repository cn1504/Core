#version 130

uniform mat4 ProjectionMatrix;

uniform vec3 SpherePosition;
uniform float Radius;
uniform vec4 DiffuseColor;
uniform vec4 SpecularColor;
uniform float SpecularHardness;
uniform vec4 EmanativeColor;

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
	
	vec4 clip_space_pos = ProjectionMatrix * vec4(qi, 1.0);
	float ndc_depth = clip_space_pos.z / clip_space_pos.w;
	float depth = (((gl_DepthRange.far-gl_DepthRange.near) * ndc_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
	gl_FragDepth = depth;
		
	gl_FragData[1] = vec4(normalize(qi - p) * 0.5 + 0.5, 1.0);
	gl_FragData[0] = DiffuseColor;
	gl_FragData[2] = SpecularColor;
	gl_FragData[3] = vec4(SpecularHardness / 16.0);
	gl_FragData[4] = EmanativeColor;
}