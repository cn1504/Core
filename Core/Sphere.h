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
			virtual btCollisionShape* GetCollisionShape(glm::vec3 scale);
		};

	}
}