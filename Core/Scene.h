#pragma once
#include "Core.h"
#include "RenderBuffer.h"
#include "Shader.h"
#include "ScreenQuad.h"
#include "Mesh.h"
#include "Camera.h"
#include "DynamicsWorld.h"
#include "Text.h"
#include "LightSource.h"

namespace Core
{
	class Window;

	class Scene
	{
	private:
		Window* Window;

		RenderBuffer* GeometryRB;
		RenderBuffer* LightRB;
		RenderBuffer* ShadowRB;
		RenderBuffer* GlowMapHorizontalRB;
		RenderBuffer* GlowMapVerticalRB;
		RenderBuffer* BufferCombineRB;
		
		// Shaders
		Shader* NOAAShader;
		Shader* FXAAShader;

		Shader* MeshShader;
		Shader* SphereShader;
		Shader* CylinderShader;

		Shader* LightShader;
		Shader* LightWithShadowShader;
		Shader* BufferCombineShader;
		Shader* FontShader;

		Shader* ShadowMeshShader;
		Shader* ShadowSphereShader;
		Shader* ShadowCylinderShader;

		Shader* BlurShader;

		ScreenQuad SQuad;

		std::vector<Entity*> Entities;
		std::vector<LightSource*> Lights;
		Mesh* Cube;
		Mesh* Sphere;
		Mesh* Cylinder;

		Text* fpsText;
		Text* consoleText;
		std::queue<std::string> consoleLines;

		// Camera Matrix Buffer
		Camera* Camera;
		glm::mat4 V;	
		glm::mat4 P;

		void RenderGeometry();
		void RenderLight();
		void RenderBloom();
		void RenderShadows();
		void RenderPost();
		void RenderUI();

	public:
		DynamicsWorld* PhysicsWorld;
		
		Scene(Core::Window* window);
		~Scene();

		void Update();
		void ResizeRenderBuffers();
		void SetActiveCamera(Core::Camera* camera);

		void AddLight(LightSource*);
		void AppendConsole(std::string msg);

		void LoadScene(std::string sceneFile);
	};

}