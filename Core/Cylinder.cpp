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
			if (Assets::Meshes.count("UnitCylinder") == 0)
			{
				Box box(0.5f);
				Assets::Meshes["UnitCylinder"] = box.GenerateMesh();
			}
			return Assets::Meshes["UnitCylinder"];
		}

	}
}