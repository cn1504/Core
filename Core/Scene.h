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

	class Scene
	{
	private:
		RenderBuffer* GeometryRB;
		RenderBuffer* LightRB;
		RenderBuffer* BufferCombineRB;

		Shader* MeshShader;
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
		glm::mat4 V;	
		glm::mat4 P;

		void RenderGeometry();
		void RenderLight();
		void RenderPost();

	public:
		DynamicsWorld* PhysicsWorld;
		Camera* Camera;
		
		Scene();
		~Scene();

		void Update();
		void ResizeRenderBuffers();
	};

}