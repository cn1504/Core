#pragma once
#include "Core.h"

namespace Core
{

	class Texture
	{
	private:
		GLuint Id;

	public:
		Texture();
		~Texture();

		GLuint GetID();
			
		void CreateTexture(bool depth, int width, int height);
		void CreateTexture(const unsigned char* bytes, int width, int height, GLint iformat, GLint format);
	};

}