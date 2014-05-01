#version 130

in vec3 Vertex;

out vec2 texCoord;

void main(void)
{
	gl_Position.xyz = Vertex;
    gl_Position.w = 1.0;
	texCoord = Vertex.xy * 0.5 + 0.5;
}