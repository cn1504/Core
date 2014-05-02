#version 130

uniform vec4 DiffuseColor;

in vec3 viewVertex;
in vec3 viewNormal;

void main(void)
{	
	gl_FragData[0] = DiffuseColor;
	gl_FragData[1] = vec4((normalize(viewNormal) + 1) * 0.5, 0.0);
}