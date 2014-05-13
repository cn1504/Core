#pragma once
#include "Core.h"
#include "RenderBuffer.h"
#include "Shader.h"
#include "ScreenQuad.h"
#include "Mesh.h"
#include "Camera.h"
#include "DynamicsWorld.h"

namespace Core
{
	class Window;

	class Scene
	{
	private:
		Window* Window;

		RenderBuffer* GeometryRB;
		RenderBuffer* LightRB;
		RenderBuffer* BufferCombineRB;

		// SMAA Textures
		Texture SMAAAreaTex;
		Texture SMAASearchTex;

		Shader* MeshShader;
		Shader* NOAAShader;
		Shader* FXAAShader;
		Shader* SphereShader;
		Shader* CylinderShader;
		Shader* LightShader;
		Shader* BufferCombineShader;

		ScreenQuad SQuad;

		std::vector<Entity*> Entities;
		Mesh* Cube;
		Mesh* Sphere;
		Mesh* Cylinder;

		// Camera Matrix Buffer
		Camera* Camera;
		glm::mat4 V;	
		glm::mat4 P;

		void RenderGeometry();
		void RenderLight();
		void RenderPost();

	public:
		DynamicsWorld* PhysicsWorld;
		
		Scene(Core::Window* window);
		~Scene();

		void Update();
		void ResizeRenderBuffers();
		void SetActiveCamera(Core::Camera* camera);
	};

}