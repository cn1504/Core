#version 400

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

// Input vertex data ; different for all executions of this shader. 
layout(location=0) in vec3 Vertex;
layout(location=1) in vec2 Uv;
layout(location=2) in vec3 Normal;
layout(location=3) in vec3 Tangent;
layout(location=4) in vec3 Bitangent;

layout(location=0) out vec4 position;

void main(void) 
{
	position	    = (ModelViewProjectionMatrix) * vec4(Vertex, 1.0);
	gl_Position     = (ModelViewProjectionMatrix) * vec4(Vertex, 1.0);
}