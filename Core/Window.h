#pragma once
#include "Core.h"
#include "Scene.h"
#include "Input.h"

namespace Core
{

	class Window
	{
	private:
		GLFWwindow* WindowPtr;

	public:
		// OpenCL Objects
		std::vector<cl::Platform> clPlatformList;
		std::vector<cl::Device> clDevices;
		cl::Context* clContext;
		cl::CommandQueue* clQueue;

		Input* Input;
		Scene* Scene;

		bool HasFocus;
		bool IsMinimized;

		Window();
		~Window();

		void LoopUntilClosed();
		void OnResize();
	};


	// GLFW Window Callbacks
	extern std::unordered_map<GLFWwindow*, Window*> WindowMap;
	void windowIconified_callback(GLFWwindow* window, int state);
	void windowFocus_callback(GLFWwindow* window, int state);
	void windowMove_callback(GLFWwindow* window, int x, int y);
	void windowResize_callback(GLFWwindow* window, int width, int height);
	void windowKeyEvent_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

}