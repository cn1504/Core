#include "RenderBuffer.h"

namespace Core
{

	RenderBuffer::RenderBuffer(glm::vec3 clearColor, int colorBufferCount, bool hasDepthTexture)
	{
		glGenFramebuffers(1, &FBO);
		ColorTextures.resize(colorBufferCount);
		for (int i = 0; i < colorBufferCount; i++)
			ColorTextures[i] = 0;
		DepthTexture = 0;
		ClearColor = clearColor;
		HasDepthTexture = hasDepthTexture;
	}


	RenderBuffer::~RenderBuffer()
	{
		glDeleteFramebuffers(1, &FBO); 
		
		glDeleteTextures((GLsizei)ColorTextures.size(), &ColorTextures[0]);
		glDeleteTextures(1, &DepthTexture);
	}


	void RenderBuffer::MakeCurrent()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, Settings::Window::Width, Settings::Window::Height);
	}


	void RenderBuffer::Clear()
	{
		glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, 1.0f);
		glClear((HasDepthTexture) ? (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT) : GL_COLOR_BUFFER_BIT);
	}


	void RenderBuffer::Rebuild()
	{
		GLenum buffers[8];
		buffers[0] = GL_COLOR_ATTACHMENT0;
		buffers[1] = GL_COLOR_ATTACHMENT1;
		buffers[2] = GL_COLOR_ATTACHMENT2;
		buffers[3] = GL_COLOR_ATTACHMENT3;
		buffers[4] = GL_COLOR_ATTACHMENT4;
		buffers[5] = GL_COLOR_ATTACHMENT5;
		buffers[6] = GL_COLOR_ATTACHMENT6;
		buffers[7] = GL_COLOR_ATTACHMENT7;
		buffers[8] = GL_COLOR_ATTACHMENT8;

		glDeleteTextures((GLsizei)ColorTextures.size(), &ColorTextures[0]);
		glDeleteTextures(1, &DepthTexture);

		if (HasDepthTexture)
			CreateTexture(DepthTexture, true, Settings::Window::Width, Settings::Window::Height);
		for (int i = 0; i < ColorTextures.size(); i++)
			CreateTexture(ColorTextures[i], false, Settings::Window::Width, Settings::Window::Height);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);
		for (int i = 0; i < ColorTextures.size(); i++)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ColorTextures[i], 0);
		glDrawBuffers((GLsizei)ColorTextures.size(), buffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Debug::Error("RenderBuffer incomplete.");
		}

		Debug::GLError("ERROR: GLError on RenderBuffer Rebuild.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void RenderBuffer::CreateTexture(GLuint & into, bool depth, int width, int height)
	{
		glGenTextures(1, &into);
		glBindTexture(GL_TEXTURE_2D, into);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			depth ? GL_DEPTH_COMPONENT : GL_RGBA16,
			width,
			height,
			0,
			depth ? GL_DEPTH_COMPONENT : GL_RGBA,
			GL_UNSIGNED_BYTE,
			NULL
			);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	

	GLuint RenderBuffer::GetOutputTexture(int i)
	{
		return ColorTextures[i];
	}


	GLuint RenderBuffer::GetDepthTexture()
	{
		return DepthTexture;
	}
}