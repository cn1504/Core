#version 330

in vec3 position;

out vec2 texCoord;

void main(void)
{
	gl_Position.xyz = position;
    gl_Position.w = 1.0;
	texCoord = position.xy * 0.5 + 0.5;
}