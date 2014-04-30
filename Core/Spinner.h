#pragma once
#include "Core.h"
#include "Component.h"
#include "Material.h"

namespace Test
{

	class Spinner :
		public Core::Component
	{
	private:
		Core::Material* mat;

	public:
		Spinner();
		virtual ~Spinner();
		virtual void Load();
		virtual void Update();
	};

}