#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{
	
	class Material
		: public Component
	{
	public:
		Material();
		virtual ~Material();
		virtual void Load();
		virtual void Update();

		glm::vec3 DiffuseColor;
	};

}