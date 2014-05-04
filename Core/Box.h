#pragma once
#include "Core.h"
#include "Shape.h"

namespace Core
{
	namespace Shapes
	{

		class Box
			: public Shape
		{
		public:
			Box();
			virtual ~Box();

			virtual Mesh* GenerateMesh();
			virtual float CalculateVolume(glm::vec3 scale);
		};

	}
}
