#pragma once
#include "Core.h"

namespace Core
{
	namespace Settings
	{

		namespace Window
		{
			extern int X;
			extern int Y;
			extern int Width;
			extern int Height;
		}

		namespace Video {
			extern float FOVY;
			extern float MinDrawDistance;
			extern float MaxDrawDistance;

			extern float MaxForegroundFPS;
			extern float MaxBackgroundFPS;
		}

		namespace Audio 
		{
			extern int AudioEnabled;
			extern int MusicEnabled;

			extern float MainVolume;
			extern float MusicVolume;
			extern float SoundEffectsVolume;
			extern float AmbientEffectsVolume;
			extern float VoiceVolume;
		}

		namespace Controls
		{
			extern int Forwards[2];
			extern int Backwards[2];
			extern int StrafeLeft[2];
		}

		namespace Misc
		{
			extern int VerboseLogging;
		}

		void Init(int argc, char* argv[]);
		void Close();
	};
}