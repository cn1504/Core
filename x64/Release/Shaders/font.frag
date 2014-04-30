#version 330
 
layout (location=0) in vec2 texcoord;

uniform sampler2D tex;
uniform vec4 color;
 
void main(void) {
	gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).a) * color;
}