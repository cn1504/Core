#include "Core.h"
#include "Window.h"
#include "AudioListener.h"

int main(int argc, char* argv[])
{
	try {
		Core::Debug::Init();

		// Initialize GLFW
		glfwSetErrorCallback(Core::Debug::error_callback);
		if (!glfwInit())
			Core::Debug::Error("GLFW failed to initialize.");


		Core::Settings::Init(argc, argv);
		Core::Time::Init();
		Core::AudioListener.Init();


		if (Core::Settings::Misc::VerboseLogging) {
			std::stringstream msg;

			// Log display information
			Core::Debug::Log("Available Monitors:");
			int monitorCount;
			GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
			for (int i = 0; i < monitorCount; i++) {
				msg << "\t" << (i + 1) << ". " << glfwGetMonitorName(monitors[i]);
				const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
				msg << " - " << mode->width << "x" << mode->height << "x" << mode->redBits * 4 << " @ " << mode->refreshRate << "Hz" << std::endl;
			}
			Core::Debug::Log(msg.str());

			Core::Debug::Log("");
			msg.str("");

			// Log OpenCL Devices
			Core::Debug::Log("OpenCL Platforms:");
			std::vector<cl::Platform> platformList;
			cl::Platform::get(&platformList);
			Core::Debug::CLError(platformList.size() != 0 ? CL_SUCCESS : -1, "cl::Platform::get");

			for (int i = 0; i < platformList.size(); i++)
			{
				std::string platformVendor;
				platformList[i].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
				msg << "\t" << (i + 1) << ". " << platformVendor << std::endl;
			}
			Core::Debug::Log(msg.str());
		}

		// Open the main window
		Core::Window mainWindow;

		Core::Debug::Close();
	}
	catch (std::runtime_error &e) {
		Core::Debug::Log(e.what());
		Core::Debug::Close();
		exit(EXIT_FAILURE);
	}
	return 0;
}