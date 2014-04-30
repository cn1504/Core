#version 330

// Interpolated input values
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;

// Uniform variables.
uniform vec4 DiffuseColor;

void main(void)
{	
	//gl_FragData [0] = vec4(1.0,1.0,1.0,1.0);
	gl_FragData [0] = DiffuseColor;
	gl_FragData [1] = vec4(inNormal, 0.0);
	//gl_FragData [2] = SpecularColor;
}