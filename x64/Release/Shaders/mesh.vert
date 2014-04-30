#version 330

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

layout(location=0) in vec3 mVertex;
layout(location=1) in vec3 mNormal;

layout(location=0) out vec3 Vertex;
layout(location=1) out vec3 Normal;

void main(void) 
{
	Vertex	        = vec3((ModelViewMatrix) * vec4(mVertex, 1.0));
	Normal			= vec3((ModelViewMatrix) * vec4(mNormal, 0.0));
	gl_Position     = (ModelViewProjectionMatrix) * vec4(mVertex, 1.0);
}