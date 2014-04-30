#version 330

uniform sampler2D compTexture;
uniform float opacity;
uniform vec2 textureOffset;
uniform vec2 textureDimensions;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 outColor;

void main(void) {
	vec4 tex = texture2D(compTexture, IN.texCoord * textureDimensions + textureOffset);
	outColor = vec4(tex.rgb * vec3(tex.w * opacity), tex.w * opacity);
	
	// outColor.w *= opacity;
	//outColor = vec4(1.0,1.0,1.0,1.0);
}