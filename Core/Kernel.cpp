#include "Kernel.h"

namespace Core
{

	Kernel::Kernel(cl::Context* context, std::vector<cl::Device>& devices, std::string filename, std::string entrypoint)
	{
		// Read in kernel file
		std::ifstream file(filename);
		Debug::CLError(file.is_open() ? CL_SUCCESS : -1, filename + " file failed to open.");
		std::string prog(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

		// Build kernel program
		cl::Program::Sources source(1, std::make_pair(prog.c_str(), prog.length() + 1));
		cl::Program program(*context, source);
		cl_int err = program.build(devices, ""); 
		
		if (err != CL_SUCCESS) {
			std::string buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
			Debug::Log(buildLog);
		}
		Debug::CLError(err, "Failed to build Kernel " + filename);
		
		// Set program entry point
		kernel = new cl::Kernel(program, entrypoint.c_str(), &err);
		Debug::CLError(err, "Kernel::Kernel()");
	}

	
	Kernel::~Kernel()
	{
		delete kernel;
	}

	
	cl::Kernel* Kernel::GetID()
	{
		return kernel;
	}

}