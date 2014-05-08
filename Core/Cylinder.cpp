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


		glm::vec3 Cylinder::CalculateCenterOfMass(glm::vec3 scale)
		{
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}


		glm::vec3 Cylinder::CalculateInertia(glm::vec3 scale)
		{
			float t1 = (1.0f / 12.0f) * scale.y * scale.y + (0.25f) * scale.x * scale.x;
			float t2 = 0.5f * scale.x;
			return glm::vec3(t1, t2, t1);
		}

	}
}