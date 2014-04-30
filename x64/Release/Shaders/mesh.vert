#version 330

uniform mat4 ModelViewProjectionMatrix;

// Input vertex data ; different for all executions of this shader. 
layout(location=0) in vec3 Vertex;
layout(location=1) in vec3 Normal;

layout(location=0) out vec4 position;
layout(location=1) out vec4 normal;

void main(void) 
{
	position	    = (ModelViewProjectionMatrix) * vec4(Vertex, 1.0);
	normal			= (ModelViewProjectionMatrix) * vec4(Normal, 0.0);
	gl_Position     = (ModelViewProjectionMatrix) * vec4(Vertex, 1.0);
}