#pragma once
#include "Core.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Font.h"

namespace Core
{
	namespace Assets
	{
		extern std::unordered_map<std::string, Mesh*> Meshes;
		extern std::unordered_map<std::string, Material*> Materials;
		extern std::unordered_map<std::string, Texture*> Textures;
		extern std::unordered_map<std::string, Font*> Fonts;

		void LoadStandardAssets();
		void CreateStandardMaterials();
		void CreateStandardTextures();
		void CreateStandardFonts();
		void Clear();
	}
}