#include "Window.h"

namespace Core
{

	std::unordered_map<GLFWwindow*, Window*> WindowMap;


	Window::Window()
	{
		// Create a windowed mode window and its OpenGL context 
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		WindowPtr = glfwCreateWindow(
			Settings::Window::Width,
			Settings::Window::Height, 
			"Core", NULL, NULL);
		if (!WindowPtr)
			Debug::Error("Failed to creating main window.");
		WindowMap[WindowPtr] = this;

		glfwSetWindowPos(WindowPtr, Settings::Window::X, Settings::Window::Y);
		glfwSetWindowFocusCallback(WindowPtr, windowFocus_callback);
		glfwSetWindowIconifyCallback(WindowPtr, windowIconified_callback);
		glfwSetWindowSizeCallback(WindowPtr, windowResize_callback);
		glfwSetWindowPosCallback(WindowPtr, windowMove_callback);
		glfwSetKeyCallback(WindowPtr, windowKeyEvent_callback);

		// Make the window's opengl context current 
		glfwMakeContextCurrent(WindowPtr);
		HasFocus = true;

		// Initialize Glew
		if (GLEW_OK != glewInit()) {
			Debug::Error("Glew Failed to Initialize.");
		}
		glGetError();

		Scene = new Core::Scene();
		Input = new Core::Input(WindowPtr);

		LoopUntilClosed();
	}


	Window::~Window()
	{
	}


	void Window::LoopUntilClosed()
	{
		while (!glfwWindowShouldClose(WindowPtr))
		{
			Scene->Update();
			glfwSwapBuffers(WindowPtr);
			Time::NextUpdate(HasFocus);
			glfwPollEvents();
			Input->Update();
		}
	}


	void Window::OnResize()
	{
		Scene->ResizeRenderBuffers();
	}


	// Window Callbacks

	void windowIconified_callback(GLFWwindow* window, int state) 
	{
		WindowMap[window]->IsMinimized = (state == GL_TRUE);
	}
	
	void windowFocus_callback(GLFWwindow* window, int state)
	{
		WindowMap[window]->HasFocus = (state == GL_TRUE);
	}

	void windowMove_callback(GLFWwindow* window, int x, int y) 
	{
		Settings::Window::X = x;
		Settings::Window::Y = y;
	}

	void windowResize_callback(GLFWwindow* window, int width, int height) 
	{
		if (width < 1)
			return;

		Settings::Window::Width = width;
		Settings::Window::Height = height;

		WindowMap[window]->OnResize();
	}

	void windowKeyEvent_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowMap[window]->Input->KeyEvent(key, scancode, action, mods);
	}
}