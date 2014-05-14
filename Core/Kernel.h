#pragma once
#include "Core.h"

namespace Core
{

	class Kernel
	{
	protected:
		cl::Kernel* kernel;

	public:
		Kernel(cl::Context* context, std::vector<cl::Device>& devices, std::string filename, std::string entrypoint);
		virtual ~Kernel();

		cl::Kernel* GetID();

		template<class T>
		cl_int SetArg(cl_uint n, T val)
		{
			return kernel->setArg(n, val);
		}

		template<class T>
		cl_int SetArg(cl_uint n, size_t size, T val)
		{
			return kernel->setArg(n, size, val);
		}
	};

}