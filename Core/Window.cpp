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
		glfwSwapInterval(Core::Settings::Video::VSync);
		HasFocus = true;

		// Initialize Glew
		if (GLEW_OK != glewInit()) {
			Debug::Error("Glew Failed to Initialize.");
		}
		glGetError();

		// Initialize OpenCL
		cl_int err;
		cl::Platform::get(&clPlatformList);
		Debug::CLError(clPlatformList.size() != 0 ? CL_SUCCESS : -1, "No OpenCL platforms found.");

		cl_context_properties cprops[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(clPlatformList[0])(), 0 };
		clContext = new cl::Context(
			CL_DEVICE_TYPE_GPU,
			cprops,
			NULL,
			NULL,
			&err);
		Debug::CLError(err, "Error establishing OpenCL context.");

		clDevices = clContext->getInfo<CL_CONTEXT_DEVICES>();
		Debug::CLError(clDevices.size() != 0 ? CL_SUCCESS : -1, "No OpenCL devices found.");

		clQueue = new cl::CommandQueue(*clContext, clDevices[0], 0, &err);
		Debug::CLError(err, "Error creating OpenCL command queue.");

		if (Settings::Misc::VerboseLogging)
		{
			std::string platformVendor;
			clPlatformList[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
			Debug::Log("Using Platform: " + std::to_string(clPlatformList.size()) + ". " + platformVendor);
			Debug::Log("");
		}

		// Load Engine objects
		Input = new Core::Input(this, WindowPtr);
		Scene = new Core::Scene(this);

		LoopUntilClosed();
	}


	Window::~Window()
	{
	}


	void Window::LoopUntilClosed()
	{
		Time::NextUpdate(HasFocus);
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