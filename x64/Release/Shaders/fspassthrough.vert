#version 330

in vec3 position;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void)
{
	gl_Position.xyz = position;
    gl_Position.w = 1.0;
	OUT.texCoord = position.xy * 0.5 + 0.5;
}