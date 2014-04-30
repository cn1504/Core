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
			Box box(0.5f);
			return Assets::Meshes["UnitSphere"] = box.GenerateMesh();
		}

	}
}