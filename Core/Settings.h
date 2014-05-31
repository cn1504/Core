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

			extern int DefaultWidth;
			extern int DefaultHeight;
		}

		namespace Video {
			extern float FOVY;
			extern float MinDrawDistance;
			extern float MaxDrawDistance;

			extern int FXAA;

			extern bool VSync;
			extern float MaxForegroundFPS;
			extern float MaxBackgroundFPS;

			extern bool ShowFPS;
			
			extern int ShadowResolution;
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

		namespace Game
		{
		}

		namespace Controls
		{
			extern int Joystick;

			extern int Forwards[2];
			extern int Backwards[2];
			extern int StrafeLeft[2];
		}

		namespace Misc
		{
			extern int VerboseLogging;
			extern bool ShowConsole;
		}

		void Init(int argc, char* argv[]);
		void Close();
	};
}