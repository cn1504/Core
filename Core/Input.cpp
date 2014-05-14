#include "Input.h"
#include "Window.h"

namespace Core
{
	Input::Input(Core::Window* window, GLFWwindow* ptr)
	{
		Window = window;
		WindowPtr = ptr;
		FreeLook = false;

		isFullscreen = false;

		int currentMonitor = 0;
		monitors = glfwGetMonitors(&monitorCount);
		GLFWmonitor* prime = glfwGetPrimaryMonitor();
		for (currentMonitor = 0; currentMonitor < monitorCount; currentMonitor++)
		{
			if (prime == monitors[currentMonitor])
				break;
		}

		JoystickCount = 0;
		for (int i = 0; i < 16; i++)
		{
			if (glfwJoystickPresent(GLFW_JOYSTICK_1 + i))
			{
				JoystickCount++;
				Joystick = GLFW_JOYSTICK_1 + i;
			}
		}

		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("Joystick count: " + std::to_string(JoystickCount));
			Debug::Log("Current joystick: " + std::to_string(Joystick) + ". " + glfwGetJoystickName(Joystick));
			Debug::Log("");
		}
	}

	Input::~Input()
	{

	}

	void Input::Update()
	{
		glm::mat4 camM = Camera->Transform.ToMatrix();
		float MoveSpeed = 5.0f;

		if (glfwGetKey(WindowPtr, GLFW_KEY_A) == 1)
		{
			Camera->Transform.Position += Camera->Transform.Rotation * (glm::vec3(MoveSpeed * Time::Delta, 0.0f, 0.0f));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_D) == 1)
		{
			Camera->Transform.Position += Camera->Transform.Rotation * (glm::vec3(-MoveSpeed * Time::Delta, 0.0f, 0.0f));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_W) == 1)
		{
			Camera->Transform.Position += Camera->Transform.Rotation * (glm::vec3(0.0f, 0.0f, MoveSpeed * Time::Delta));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_S) == 1)
		{
			Camera->Transform.Position += Camera->Transform.Rotation * (glm::vec3(0.0f, 0.0f, -MoveSpeed * Time::Delta));
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_LEFT) == 1)
		{
			CameraRotation.y += 0.5f * Time::RealtimeDelta;
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_RIGHT) == 1)
		{
			CameraRotation.y -= 0.5f * Time::RealtimeDelta;
		}
	
		if (glfwGetKey(WindowPtr, GLFW_KEY_UP) == 1)
		{
			CameraRotation.x -= 0.5f * Time::RealtimeDelta;
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_DOWN) == 1)
		{
			CameraRotation.x += 0.5f * Time::RealtimeDelta;
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_SPACE) == 1)
		{
			Camera->Transform.Position += glm::vec3(0.0f, MoveSpeed * Time::Delta, 0.0f);
		}

		if (glfwGetKey(WindowPtr, GLFW_KEY_LEFT_SHIFT) == 1)
		{
			Camera->Transform.Position += glm::vec3(0.0f, -MoveSpeed * Time::Delta, 0.0f);
		}


		// Mouse
		if (FreeLook)
		{
			double x, y;
			glfwGetCursorPos(WindowPtr, &x, &y);
			glm::vec2 CurrentMousePosition = glm::vec2(x, y);
			glm::vec2 deltaP = CurrentMousePosition - MousePosition;
			CameraRotation.y -= deltaP.x * 0.003f;
			CameraRotation.x += deltaP.y * 0.003f;
			MousePosition = CurrentMousePosition;
		}


		// Joystick
		if (glfwJoystickPresent(Joystick))
		{
			int joyAxesCount;
			int joyButtonsCount;
			auto joyAxes = glfwGetJoystickAxes(Joystick, &joyAxesCount);
			auto joyButtons = glfwGetJoystickButtons(Joystick, &joyButtonsCount);
			float joyMoveSpeed = 5.0f;
			float joyTurnSpeed = 2.0f;
			float joyDeadZone = 0.2f;

			if (joyAxesCount >= 2) {
				if (joyAxes[0] > joyDeadZone || joyAxes[0] < -joyDeadZone)
					Camera->Transform.Position -= Camera->Transform.Rotation * (glm::vec3(joyAxes[0] * joyMoveSpeed * Time::Delta, 0.0f, 0.0f));
				if (joyAxes[1] > joyDeadZone || joyAxes[1] < -joyDeadZone)
					Camera->Transform.Position += Camera->Transform.Rotation * (glm::vec3(0.0f, 0.0f, joyAxes[1] * joyMoveSpeed * Time::Delta));
			}
			if (joyAxesCount >= 5) {
				if (joyAxes[3] > joyDeadZone || joyAxes[3] < -joyDeadZone)
				{
					CameraRotation.x += joyAxes[3] * joyTurnSpeed * Time::RealtimeDelta;
				}
				if (joyAxes[4] > joyDeadZone || joyAxes[4] < -joyDeadZone)
				{
					CameraRotation.y -= joyAxes[4] * joyTurnSpeed * Time::RealtimeDelta;
				}
			}
		}

		// Apply final camera rotation
		if (CameraRotation.x > glm::pi<float>() / 2)
			CameraRotation.x = glm::pi<float>() / 2;
		if (CameraRotation.x < -glm::pi<float>() / 2)
			CameraRotation.x = -glm::pi<float>() / 2;
		Camera->Transform.Rotation = glm::quat(CameraRotation);
	}


	void Input::KeyEvent(int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			ToggleFreelook();
		}

		else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && GLFW_MOD_ALT & mods)
		{
			isFullscreen = !isFullscreen;
			RepositionWindow();
		}

		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && GLFW_MOD_ALT & mods)
		{
			currentMonitor--;
			if (currentMonitor < 0)
				currentMonitor = monitorCount - 1;
			RepositionWindow();
		}

		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && GLFW_MOD_ALT & mods)
		{
			currentMonitor++;
			if (currentMonitor == monitorCount)
				currentMonitor = 0;
			RepositionWindow();
		}


		else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
		{
			Settings::Video::FXAA = (Settings::Video::FXAA + 1) % 2;
		}

	}


	void Input::ToggleFreelook()
	{
		FreeLook = !FreeLook;
		glfwSetInputMode(WindowPtr, GLFW_CURSOR, (FreeLook) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		double x, y;
		glfwGetCursorPos(WindowPtr, &x, &y);
		MousePosition = glm::vec2(x, y);
	}


	void Input::RepositionWindow()
	{
		if (FreeLook)
			ToggleFreelook();

		if (!isFullscreen)
		{
			int xpos, ypos;
			glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);
			auto vm = glfwGetVideoMode(monitors[currentMonitor]);

			glfwSetWindowSize(WindowPtr, Settings::Window::DefaultWidth, Settings::Window::DefaultHeight);
			glfwSetWindowPos(WindowPtr, xpos + vm->width / 2 - Settings::Window::DefaultWidth / 2, ypos + vm->height / 2 - Settings::Window::DefaultHeight / 2);
		}
		else
		{
			int xpos, ypos;
			glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);
			auto vm = glfwGetVideoMode(monitors[currentMonitor]);

			glfwSetWindowSize(WindowPtr, vm->width, vm->height);
			glfwSetWindowPos(WindowPtr, xpos, ypos);
		}
	}

	
	void Input::SetPlayerEntity(Entity* player)
	{
		Player = player;
	}


	void Input::SetCameraEntity(Entity* camera)
	{
		Camera = camera;
	}

}