#pragma once
#include "Core.h"

namespace Core
{

	static class AL
	{
	private:
		ALCdevice *device;
		ALCcontext *ctx;

	public:
		void Init();
		void Update();
		void Close();
	} AudioListener;

}