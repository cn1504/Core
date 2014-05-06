#version 130

uniform sampler2D NormalTexture;
uniform sampler2D DepthTexture;
uniform sampler2D HardnessTexture;

uniform vec2      PixelSize;
uniform mat4      ProjectionInverse;

uniform float     LightRadius;
uniform vec3      LightPosition;
uniform vec4      LightColor;

in vec3 viewVertex;
in vec3 viewNormal;

void main(void)
{
	// Extract buffered pixel position and normal from textures
	vec3 pos 		= vec3((gl_FragCoord.x * PixelSize.x), 
						   (gl_FragCoord.y * PixelSize.y), 0.0);
	pos.z           = texture(DepthTexture, pos.xy).r;
	vec3 normal     = normalize(texture(NormalTexture, pos.xy).xyz * 2.0 - 1.0);
	float hardness  = texture(HardnessTexture, pos.xy).r * 16;
	vec4 clip       = ProjectionInverse * vec4(pos * 2.0 - 1.0, 1.0);
	pos             = clip.xyz / clip.w;
	
	// Calculate light amount
	vec3 lightDir   = LightPosition - pos;
	float dist      = length(lightDir) / LightRadius;	
	float atten     = clamp(1.0 - dist * dist, 0.0, 1.0);
	vec3 incident   = normalize(lightDir);
	
	if(atten == 0.0) 
	{
		discard;
	}
	if (dot(normal, incident) < 0.0) 
    {
		discard;
    }
	
	vec3 viewDir    = normalize(- pos);
	
	float lambert   = clamp(dot(incident, normal), 0.0, 1.0);

	float sFactor;
	if (dot(normal, incident) < 0.0) 
    {
		sFactor = 0.0;
    }
	else
    {
		sFactor = pow(max(0.0, dot(reflect(-incident, normal), viewDir)), 5.0); // buggy hardness);
    }

	gl_FragData[0] = vec4(LightColor.xyz * lambert * atten, 1.0);
	gl_FragData[1] = vec4(LightColor.xyz * sFactor * atten, 1.0);
}