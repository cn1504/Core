#pragma once
#include "Core.h"
#include "Scene.h"
#include "Entity.h"

namespace Core
{
	class Input
	{
	private:
		Window* Window;
		GLFWwindow* WindowPtr;
		bool isFullscreen;
		int currentMonitor;
		int monitorCount; 
		GLFWmonitor** monitors;

		Entity* Camera;
		Entity* Player;

		bool FreeLook;
		glm::vec3 CameraRotation; 
		glm::vec2 MousePosition;

		void ToggleFreelook();
		void RepositionWindow();

		// Joystick / Gamepad
		int JoystickCount;
		int Joystick;	// Current Joystick	

		std::string consoleOutput;

	public:
		Input(Core::Window* window, GLFWwindow* ptr);
		~Input();

		void Update();
		void KeyEvent(int key, int scancode, int action, int mods);

		void SetPlayerEntity(Entity* player);
		void SetCameraEntity(Entity* player);
	};
}