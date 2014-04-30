#version 330

uniform sampler2D diffuseTex;
uniform sampler2D specularColorTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;
uniform sampler2D glowmapTex;
uniform sampler2D skyboxTexture;
uniform vec3 ambientLight; 
uniform float dynamicRange;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 outColor;

void main(void)
{
	vec3 diffuse   = texture(diffuseTex, IN.texCoord).xyz;
	//vec3 specularC = texture(specularColorTex, IN.texCoord).xyz;
	//vec3 light     = texture(emissiveTex, IN.texCoord).xyz;
	//vec3 specular  = texture(specularTex, IN.texCoord).xyz;
	//vec3 glow      = texture(glowmapTex, IN.texCoord).xyz;
	
	//vec3 baseColor = diffuse * ambientLight + 			// ambient
	//				 diffuse * light * dynamicRange;		// diffused light
	
	//outColor.xyz   = (baseColor == vec3(0.0)) ? texture(skyboxTexture, IN.texCoord).xyz : baseColor;
	
	vec3 baseColor = diffuse; // diffused light
	outColor.xyz   = baseColor;
					 
	//outColor.xyz += specular * specularC * dynamicRange;	// specular
	//outColor.xyz += glow;
	outColor.a    = 1.0;
	
	
	//outColor = vec4(glow, 1.0);
}