#pragma once
#include "Core.h"
#include "Mesh.h"
#include "Material.h"

namespace Core
{
	namespace Assets
	{
		extern std::unordered_map<std::string, Mesh*> Meshes;
		extern std::unordered_map<std::string, Material*> Materials;

		void CreateStandardMaterials();
		void Clear();
	}
}