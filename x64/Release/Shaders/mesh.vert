#version 130

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

in vec3 Vertex;
in vec3 Normal;

out vec3 viewVertex;
out vec3 viewNormal;

void main(void) 
{
	viewVertex	        = vec3((ModelViewMatrix) * vec4(Vertex, 1.0));
	viewNormal			= vec3((ModelViewMatrix) * vec4(Normal, 0.0));
	gl_Position         = (ModelViewProjectionMatrix) * vec4(Vertex, 1.0);
}