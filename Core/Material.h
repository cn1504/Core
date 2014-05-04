#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{
	
	class Material
		: public Component
	{
	public:
		Material(std::string name);
		virtual ~Material();
		virtual void Load();
		virtual void Update();

		glm::vec3 DiffuseColor;
		float DiffuseIntensity;
		glm::vec3 SpecularColor;
		float SpecularIntensity;
		float SpecularHardness;
		float IndexOfRefraction;

		float Density;

		float YoungsModulus;
		float BulkModulus;
		float ShearModulus;

		float YieldStrength;
		float UltimateStrength;
	};

}