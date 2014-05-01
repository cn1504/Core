#version 130

uniform vec4 DiffuseColor;

in vec3 Vertex;
in vec3 Normal;

void main(void)
{	
	gl_FragData[0] = DiffuseColor;
	gl_FragData[1] = vec4(Normal, 0.0);
}