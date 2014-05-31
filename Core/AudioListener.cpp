#include "AudioListener.h"

namespace Core
{

	void AL::Init()
	{
		device = NULL;
		ctx = NULL;

		/* Open and initialize a device with default settings */
		device = alcOpenDevice(NULL);
		if (!device)
		{
			Settings::Audio::AudioEnabled = 0;
			return;
		}

		ctx = alcCreateContext(device, NULL);
		if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
		{
			if (ctx != NULL)
				alcDestroyContext(ctx);
			alcCloseDevice(device);

			Settings::Audio::AudioEnabled = 0;
			return;
		}

		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("Audio Device: " + std::string(alcGetString(device, ALC_DEVICE_SPECIFIER)) + "\n");
		}
	}


	void AL::Close(void)
	{
		ALCdevice *device;
		ALCcontext *ctx;

		ctx = alcGetCurrentContext();
		if (ctx == NULL)
			return;

		device = alcGetContextsDevice(ctx);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(ctx);
		alcCloseDevice(device);
	}


	void AL::Update()
	{
		
	}

}