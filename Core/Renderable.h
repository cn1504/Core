#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{

	class Renderable
		: public Component
	{
	public:
		virtual ~Renderable() {}
		virtual void Load() = 0;
		virtual void Update() = 0;

		virtual void EnableBuffers() = 0;
		virtual void Render() = 0;
		virtual void DisableBuffers() = 0;
	};

}