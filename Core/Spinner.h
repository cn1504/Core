#pragma once
#include "Core.h"
#include "Component.h"
#include "Material.h"
#include "RigidBody.h"

namespace Test
{

	class Spinner :
		public Core::Component
	{
	private:
		Core::RigidBody* FB;
		float TimeSinceLastImpulse;
		int i;

	public:
		Spinner();
		virtual ~Spinner();
		virtual void Load();
		virtual void Update();
	};

}