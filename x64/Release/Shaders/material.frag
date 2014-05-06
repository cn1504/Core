#version 130

uniform mat4 ProjectionMatrix;

uniform vec4 DiffuseColor;
uniform vec4 SpecularColor;
uniform float SpecularHardness;

in vec3 viewVertex;
in vec3 viewNormal;

void main(void)
{	
	gl_FragData[0] = DiffuseColor;
	gl_FragData[1] = vec4(normalize(viewNormal) * 0.5 + 0.5, 1.0);
	gl_FragData[2] = SpecularColor;
	gl_FragData[3] = vec4(SpecularHardness / 16.0);
}