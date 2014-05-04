#include "Cylinder.h"
#include "Box.h"
#include "Assets.h"

namespace Core
{
	namespace Shapes
	{

		Cylinder::Cylinder()
		{
		}


		Cylinder::~Cylinder()
		{
		}


		Mesh* Cylinder::GenerateMesh()
		{
			Box box;
			return box.GenerateMesh();
		}


		float Cylinder::CalculateVolume(glm::vec3 scale)
		{
			float r = scale.x / 2.0f;
			return glm::pi<float>() * r * r * scale.y;
		}

	}
}