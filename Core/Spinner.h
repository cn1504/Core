#pragma once
#include "Core.h"
#include "Component.h"
#include "Material.h"
#include "FreeBody.h"

namespace Test
{

	class Spinner :
		public Core::Component
	{
	private:
		Core::FreeBody* FB;
		float TimeSinceLastImpulse;
		int i;

	public:
		Spinner();
		virtual ~Spinner();
		virtual void Load();
		virtual void Update();
	};

}