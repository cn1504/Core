#include "Texture.h"

namespace Core
{

	Texture::Texture()
	{
		Id = 0;
	}


	Texture::~Texture()
	{
		glDeleteTextures(1, &Id);
	}


	GLuint Texture::GetID()
	{
		return Id;
	}

	
	void Texture::CreateTexture(bool depth, int width, int height)
	{
		// Clear old texture if it exists
		glDeleteTextures(1, &Id);

		// Build new texture
		glGenTextures(1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			depth ? GL_DEPTH_COMPONENT : GL_RGBA,
			width,
			height,
			0,
			depth ? GL_DEPTH_COMPONENT : GL_RGBA,
			GL_UNSIGNED_BYTE,
			NULL
			);

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void Texture::CreateTexture(const unsigned char* bytes, int width, int height, GLint iformat, GLint format)
	{
		// Clear old texture if it exists
		glDeleteTextures(1, &Id);

		// Build new texture
		glGenTextures(1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			iformat,
			width,
			height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			bytes
			);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}