#pragma once
#include "Core.h"
#include "RenderBuffer.h"
#include "Shader.h"
#include "ScreenQuad.h"
#include "Mesh.h"
#include "Camera.h"

namespace Core
{

	class Scene
	{
	private:
		RenderBuffer* GeometryRB;
		Shader* MeshShader;
		Shader* FXAAShader;
		Shader* SphereShader;
		ScreenQuad SQuad;

		std::vector<Entity*> Entities;

	public:
		Camera* Camera;
		
		Scene();
		~Scene();

		void Update();
		void ResizeRenderBuffers();
	};

}