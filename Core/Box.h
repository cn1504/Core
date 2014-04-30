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
		private:
			glm::vec3 HalfLengths;

		public:
			Box(float cubeHalfLength);
			Box(float halfLengthX, float halfLengthY, float halfLengthZ);
			Box(glm::vec3 halfLengths);
			virtual ~Box();

			virtual Mesh* GenerateMesh();
		};

	}
}
