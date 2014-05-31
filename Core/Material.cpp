#include "Material.h"
#include "Assets.h"

namespace Core
{

	Material::Material(std::string name)
	{
		Assets::Materials[name] = this;

		DiffuseColor = glm::vec3(1.0f);
		Density = 0.0f;

		EmanativeIntensity = 0.0f;
	}


	Material::~Material()
	{
	}


	void Material::Load()
	{
	}


	void Material::Update()
	{
	}
	
}