#include "Sphere.h"
#include "Box.h"
#include "Assets.h"

namespace Core
{
	namespace Shapes
	{

		Sphere::Sphere()
		{
		}
		
		
		Sphere::~Sphere()
		{
		}


		Mesh* Sphere::GenerateMesh()
		{
			Box box;
			return box.GenerateMesh();
		}


		float Sphere::CalculateVolume(glm::vec3 scale)
		{
			float r = scale.x / 2.0f;
			return (4.0f / 3.0f) * glm::pi<float>() * r * r * r;
		}


		btCollisionShape* Sphere::GetCollisionShape(glm::vec3 scale)
		{
			return new btSphereShape(btScalar(scale.x / 2.0f));
		}

	}
}