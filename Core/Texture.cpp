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


	void Texture::CreateTexture(const unsigned char* bytes, int width, int height, GLint iformat, GLint format, GLfloat wrap, GLfloat mag_filter, GLfloat min_filter)
	{
		// Clear old texture if it exists
		glDeleteTextures(1, &Id);

		// Build new texture
		glGenTextures(1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		// Add Anisotropic Filtering
		GLfloat maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

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


	void Texture::CreateCubeMap(bool depth, int width, int height)
	{
		// Clear old texture if it exists
		glDeleteTextures(1, &Id);

		// Build new texture
		glGenTextures(1, &Id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if (depth)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		}

		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i,
				0,										//level
				depth ? GL_DEPTH_COMPONENT32F : GL_RGBA,   //internal format
				width,									//width
				height,									//height
				0,										//border
				depth ? GL_DEPTH_COMPONENT : GL_RGBA,	//format
				depth ? GL_FLOAT : GL_UNSIGNED_BYTE,		//type
				0);									// pixel data
		}

		Debug::GLError("Error creating CubeMap texture.");
	}


	void Texture::LoadFromPNG(const std::string& pngFilename, GLuint width, GLuint height)
	{
		std::vector<unsigned char> image;

		unsigned error = LibTexture::LoadPNG(image, width, height, pngFilename);

		// If there's an error, display it.
		if (error != 0)
		{
			Debug::Error("Error loading png texture file: " + std::to_string(error));
		}

		CreateTexture(&image[0], width, height, GL_RGBA, GL_RGBA, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	}

}