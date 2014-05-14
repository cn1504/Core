#include "Debug.h"

namespace Core
{
	namespace Debug {
		void Init() {
			logFile.open(logFilename, std::ios::out);
		}

		void Error(std::string error_message)
		{
			Log(error_message);
			Close();
			exit(EXIT_FAILURE);
		}

		void GLError(std::string error_message)
		{
			const GLenum ErrorValue = glGetError();

			if (ErrorValue != GL_NO_ERROR) {
				Log("GL Error: \"" + std::to_string(ErrorValue) + "\": " + error_message);
				Close();
				exit(EXIT_FAILURE);
			}
		}

		void CLError(cl_int error, std::string error_message)
		{
			if (error != CL_SUCCESS) {
				Log("OpenCL Error: " + std::to_string(error) + ". " + error_message);
				Close();
				exit(EXIT_FAILURE);
			}
		}

		void Log(std::string message) {
			logFile << message << std::endl;
		}

		void Close() 
		{
			glfwTerminate();
			Settings::Close();
			logFile.close();
		}

		void error_callback(int error, const char* description) {
			Error(description);
		}
	}
}
