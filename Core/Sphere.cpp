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
			if (Assets::Meshes.count("UnitSphere") == 0)
			{
				Box box(0.5f);
				Assets::Meshes["UnitSphere"] = box.GenerateMesh();
			}
			return Assets::Meshes["UnitSphere"];
		}

	}
}