#version 400

uniform sampler2D depthTex;
uniform sampler2D normTex;

uniform mat4 ModelMatrix;
uniform mat4 lightViewProjection;

uniform vec2      pixelSize;
uniform vec3      cameraPos;
uniform float     dynamicRange;

uniform float     lightRadius;
uniform vec3      lightPosition;
uniform vec4      lightColor;
uniform vec3      lightDirection;
uniform float     lightOuterCutoff;
uniform float     lightInnerCutoff;
uniform int       lightCastsShadow;
uniform sampler2D lightShadowMap;

uniform mat4      ViewProjectionInverse;

layout(location=0) in vec4 position;

// The scale matrix is used to push the projected vertex into the 0.0 - 1.0 region.
// Similar in role to a * 0.5 + 0.5, where -1.0 < a < 1.0.
const mat4 ScaleMatrix = mat4(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

void main(void)
{
	vec3 pos 		= vec3((gl_FragCoord.x * pixelSize.x), 
						   (gl_FragCoord.y * pixelSize.y), 0.0);
	pos.z           = texture(depthTex, pos.xy).r;
	
	vec3 normal     = normalize(texture(normTex, pos.xy).xyz * 2.0 - 1.0);
	
	vec4 clip       = ViewProjectionInverse * vec4(pos * 2.0 - 1.0, 1.0);
	pos             = clip.xyz / clip.w;
	
	float dist      = length(lightPosition - pos) / lightRadius;
	
	float atten     = clamp(1.0 - dist * dist, 0.0, 1.0);
		
	if(atten == 0.0) 
	{
		discard;
	}
	
	vec3 incident   = normalize(lightPosition - pos);
	vec3 viewDir    = normalize(cameraPos - pos);
	vec3 halfDir    = normalize(incident + viewDir);
	
	float spotlight = max(-dot(incident, lightDirection), 0.0);
	float spotlightFade = clamp((lightOuterCutoff - spotlight) / (lightOuterCutoff - lightInnerCutoff), 0.0, 1.0);
	atten = atten * spotlight * spotlightFade;
	
	if(atten == 0.0) 
	{
		discard;
	}
	
	float shadow = 1.0;
	if(lightCastsShadow != 0)
	{
		vec4 shadowCoords = ScaleMatrix * lightViewProjection * vec4(pos, 1.0);
		float shadowDepth = textureProj(lightShadowMap, shadowCoords).r;
	
		// ESM
		const float c = 60.0; // Sharp shadows good for interior scenes
		//const float c = 5.0; 	// Soft shadows, good for day time exterior scenes
		shadow = clamp(exp(-c * ( dist - shadowDepth )), 0.0, 1.0);
	}
	
	float lambert   = clamp(dot(incident, normal), 0.0, 1.0);
						
	vec3 reflection = normalize(reflect(-incident, normal));
	float sFactor   = pow(max(dot(reflection, viewDir), 0.0), 5);
		
	gl_FragData[0] = vec4(lightColor.xyz * lambert * atten * shadow / dynamicRange, 1.0);
	gl_FragData[1] = vec4(lightColor.xyz * sFactor * atten * shadow / dynamicRange, 1.0);
}