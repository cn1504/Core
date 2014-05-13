#version 130

uniform sampler2D sourceTexture;

in vec2 texCoord;

out vec4 FragColor;

void main(void)
{	
	FragColor = texture2D(sourceTexture, texCoord);
}