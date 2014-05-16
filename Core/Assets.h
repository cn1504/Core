#pragma once
#include "Core.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

namespace Core
{
	namespace Assets
	{
		extern std::unordered_map<std::string, Mesh*> Meshes;
		extern std::unordered_map<std::string, Material*> Materials;
		extern std::unordered_map<std::string, Texture*> Textures;

		void CreateStandardMaterials();
		void CreateStandardTextures();
		void Clear();
	}
}