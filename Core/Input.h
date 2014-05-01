#pragma once
#include "Core.h"
#include "Scene.h"

namespace Core
{
	class Input
	{
	private:
		GLFWwindow* WindowPtr;
		bool isFullscreen;
		int currentMonitor;
		int monitorCount; 
		GLFWmonitor** monitors;

		Scene* Scene;

		bool FreeLook;
		glm::vec3 CameraRotation; 
		glm::vec2 MousePosition;

		void ToggleFreelook();
		void RepositionWindow();

	public:
		Input(GLFWwindow* window);
		~Input();

		void Update();
		void KeyEvent(int key, int scancode, int action, int mods);
	};
}