#pragma once
#include "Core.h"
#include "Mesh.h"

namespace Core
{

	class Shape
	{
	public:
		virtual ~Shape() {}
		virtual Mesh* GenerateMesh() = 0;
	};

}