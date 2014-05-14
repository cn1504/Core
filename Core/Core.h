// Standard Engine Includes

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	// Hides the console window

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <memory>
#include <functional>
#include <chrono>
#include <stdexcept>
#include <thread>

#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include <CL/cl.hpp>
#pragma comment(lib, "OpenCL.lib")

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <glm/glm.hpp>
#include <glm/ext.hpp>
//#include <glm/gtx/quaternion.hpp>

#include "Exceptions.h"
#include "Settings.h"
#include "Debug.h"
#include "Time.h"

namespace Core
{
	// Forward Declarations
	class Entity;
	class Component;
	class Renderable;
}
