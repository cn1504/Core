#version 330

layout (location=0) in vec4 coord;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void) {
	gl_Position = vec4(coord.xy, 0, 1);
	OUT.texCoord = coord.zw;
}