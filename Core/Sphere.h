#pragma once
#include "Core.h"
#include "Shape.h"

namespace Core
{
	namespace Shapes
	{

		class Sphere
			: public Shape
		{
		public:
			Sphere();
			virtual ~Sphere();

			virtual Mesh* GenerateMesh();
			virtual float CalculateVolume(glm::vec3 scale);
			virtual glm::vec3 CalculateCenterOfMass(glm::vec3 scale);
			virtual glm::vec3 CalculateInertia(glm::vec3 scale);
		};

	}
}