#include "Input.h"
#include "Window.h"

namespace Core
{
	Input::Input(GLFWwindow* window)
	{
		WindowPtr = window;
		Scene = WindowMap[WindowPtr]->Scene;
		FreeLook = false;
	}

	Input::~Input()
	{

	}

	void Input::Update()
	{
		glm::mat4 camM = Scene->Camera->Entity->Transform.ToMatrix();
		float MoveSpeed = 5.0f;

		if (glfwGetKey(WindowPtr, GLFW_KEY_A) == 1)
		{
			Scene->Camera->Entity->Transform.Position += Scene->Camera->Entity->Transform.Rotation * (glm::vec3(MoveSpeed * Time::Delta, 0.0f, 0.0f));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_D) == 1)
		{
			Scene->Camera->Entity->Transform.Position += Scene->Camera->Entity->Transform.Rotation * (glm::vec3(-MoveSpeed * Time::Delta, 0.0f, 0.0f));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_W) == 1)
		{
			Scene->Camera->Entity->Transform.Position += Scene->Camera->Entity->Transform.Rotation * (glm::vec3(0.0f, 0.0f, MoveSpeed * Time::Delta));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_S) == 1)
		{
			Scene->Camera->Entity->Transform.Position += Scene->Camera->Entity->Transform.Rotation * (glm::vec3(0.0f, 0.0f, -MoveSpeed * Time::Delta));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_LEFT) == 1)
		{
			CameraRotation.y += 0.5f * Time::RealtimeDelta;
			Scene->Camera->Entity->Transform.Rotation = glm::quat(CameraRotation);
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_RIGHT) == 1)
		{
			CameraRotation.y -= 0.5f * Time::RealtimeDelta;
			Scene->Camera->Entity->Transform.Rotation = glm::quat(CameraRotation);
		}
	
		if (glfwGetKey(WindowPtr, GLFW_KEY_UP) == 1)
		{
			CameraRotation.x -= 0.5f * Time::RealtimeDelta;
			Scene->Camera->Entity->Transform.Rotation = glm::quat(CameraRotation);
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_DOWN) == 1)
		{
			CameraRotation.x += 0.5f * Time::RealtimeDelta;
			Scene->Camera->Entity->Transform.Rotation = glm::quat(CameraRotation);
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_SPACE) == 1)
		{
			Scene->Camera->Entity->Transform.Position += glm::vec3(0.0f, MoveSpeed * Time::Delta, 0.0f);
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_LEFT_SHIFT) == 1)
		{
			Scene->Camera->Entity->Transform.Position += glm::vec3(0.0f, -MoveSpeed * Time::Delta, 0.0f);
		}

		// Mouse
		if (FreeLook)
		{
			double x, y;
			glfwGetCursorPos(WindowPtr, &x, &y);
			glm::vec2 CurrentMousePosition = glm::vec2(x, y);
			glm::vec2 deltaP = CurrentMousePosition - MousePosition;
			CameraRotation.y -= deltaP.x * 0.005f;
			CameraRotation.x += deltaP.y * 0.005f;
			MousePosition = CurrentMousePosition;
			Scene->Camera->Entity->Transform.Rotation = glm::quat(CameraRotation);
		}
	}


	void Input::KeyEvent(int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			FreeLook = !FreeLook;
			glfwSetInputMode(WindowPtr, GLFW_CURSOR, (FreeLook) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

			double x, y;
			glfwGetCursorPos(WindowPtr, &x, &y);
			MousePosition = glm::vec2(x, y);
		}
	}
}