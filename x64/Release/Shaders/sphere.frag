#version 330

// Uniform variables.
uniform mat4 ModelViewProjectionMatrix;
uniform vec3 CameraPosition;
uniform vec3 SpherePosition;
uniform vec4 DiffuseColor;
uniform float Radius;

// Interpolated input values
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;

void main(void)
{	
	float r = Radius;
	vec3 o = SpherePosition - CameraPosition;
	vec3 d = normalize(inVertex - CameraPosition);
	
	//Compute A, B and C coefficients
	float a = dot(d, d);
    float b = 2 * dot(d, o);
    float c = dot(o, o) - (r * r);
	
	//Find discriminant
    float disc = b * b - 4 * a * c;
    
    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0)
        discard;
	
	gl_FragData [0] = DiffuseColor;
	gl_FragData [1] = vec4(inNormal, 0.0);
	//gl_FragData [2] = SpecularColor;
}