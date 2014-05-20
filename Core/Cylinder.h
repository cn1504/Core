#pragma once
#include "Core.h"
#include "Shape.h"

namespace Core
{
	namespace Shapes
	{

		class Cylinder
			: public Shape
		{
		public:
			Cylinder();
			virtual ~Cylinder();

			virtual Mesh* GenerateMesh();
			virtual float CalculateVolume(glm::vec3 scale);
			virtual btCollisionShape* GetCollisionShape(glm::vec3 scale);
		};

	}
}