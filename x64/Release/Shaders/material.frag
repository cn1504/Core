#version 330

uniform vec4 DiffuseColor;

layout(location=0) in vec3 Vertex;
layout(location=1) in vec3 Normal;

layout(location=0) out vec4 diffuseOut;
layout(location=1) out vec4 normalOut;

void main(void)
{	
	diffuseOut = DiffuseColor;
	normalOut = vec4(Normal, 0.0);
}