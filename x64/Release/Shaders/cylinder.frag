#version 130

uniform mat4 ProjectionMatrix;

uniform vec3 Direction;
uniform vec3 Center;
uniform float Length;
uniform float Radius;
uniform vec4 DiffuseColor;
uniform vec4 SpecularColor;
uniform float SpecularHardness;

in vec3 viewVertex;
in vec3 viewNormal;

void main(void)
{	
	float r = Radius;
	
	vec3 o = vec3(0.0, 0.0, 0.0);
	vec3 v = normalize(viewVertex);
	vec3 p0 = Center - (Length/2)*Direction;
	vec3 p1 = Center + (Length/2)*Direction;
	vec3 u = normalize(p1 - p0);
	
	if((abs(dot(u, normalize(viewNormal))) > 0.9999) &&
		(length(viewVertex - p0) < r || length(viewVertex - p1) < r))
	{
		gl_FragData[1] = vec4(normalize(viewNormal) * 0.5 + 0.5, 1.0);
		
		vec4 clip_space_pos = ProjectionMatrix * vec4(viewVertex, 1.0);
		float ndc_depth = clip_space_pos.z / clip_space_pos.w;
		float depth = (((gl_DepthRange.far-gl_DepthRange.near) * ndc_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
		gl_FragDepth = depth;
	}
	else
	{
		vec3 AB = (p1 - p0);
		vec3 AO = (o - p0);
		vec3 AOxAB = cross(AO, AB);
		vec3 VxAB  = cross(v, AB);
		float ab2  = dot(AB, AB);
		float a    = dot(VxAB, VxAB);
		float b    = 2 * dot(VxAB, AOxAB);
		float c    = dot(AOxAB, AOxAB) - (r*r * ab2);
		
		float det = b*b - 4*a*c;
		if(det < 0)
			discard;
		
		float sqrtDet = sqrt(det);
		float t1 = (-b - sqrtDet)/(2*a);
		float t2 = (-b + sqrtDet)/(2*a);
		float t = min(t1, t2);
		
		vec3 qi = v*t;
		float pi = dot(qi - p0, u);
		
		if(pi < 0 || pi > Length)
		{
			discard;
		}
		
		vec3 ui = p0 + pi * u;
		
		gl_FragData[1] = vec4(normalize(qi - ui) * 0.5 + 0.5, 1.0);
		
		vec4 clip_space_pos = ProjectionMatrix * vec4(qi, 1.0);
		float ndc_depth = clip_space_pos.z / clip_space_pos.w;
		float depth = (((gl_DepthRange.far-gl_DepthRange.near) * ndc_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
		gl_FragDepth = depth;
	}
	
	gl_FragData[0] = DiffuseColor;
	gl_FragData[2] = SpecularColor;
	gl_FragData[3] = vec4(SpecularHardness / 16.0);
}