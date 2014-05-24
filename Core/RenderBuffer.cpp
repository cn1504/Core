#include "RenderBuffer.h"

namespace Core
{

	RenderBuffer::RenderBuffer(glm::vec4 clearColor, int colorBufferCount, bool hasDepthTexture)
	{
		glGenFramebuffers(1, &FBO);
		ColorTextures.resize(colorBufferCount);
		ClearColor = clearColor;
		HasDepthTexture = hasDepthTexture;
	}


	RenderBuffer::~RenderBuffer()
	{
		glDeleteFramebuffers(1, &FBO); 
	}


	void RenderBuffer::MakeCurrent()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, Settings::Window::Width, Settings::Window::Height);
	}


	void RenderBuffer::Clear()
	{
		glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.a);
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
		
		if (HasDepthTexture)
			DepthTexture.CreateTexture(true, Settings::Window::Width, Settings::Window::Height);
		for (int i = 0; i < ColorTextures.size(); i++)
			ColorTextures[i].CreateTexture(false, Settings::Window::Width, Settings::Window::Height);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture.GetID(), 0);
		for (int i = 0; i < ColorTextures.size(); i++)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ColorTextures[i].GetID(), 0);
		glDrawBuffers((GLsizei)ColorTextures.size(), buffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Debug::Error("RenderBuffer incomplete.");
		}

		Debug::GLError("ERROR: GLError on RenderBuffer Rebuild.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	

	GLuint RenderBuffer::GetOutputTexture(int i)
	{
		return ColorTextures[i].GetID();
	}


	GLuint RenderBuffer::GetDepthTexture()
	{
		return DepthTexture.GetID();
	}



	ShadowRenderBuffer::ShadowRenderBuffer()
		: RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 0, true)
	{
	}

	ShadowRenderBuffer::~ShadowRenderBuffer()
	{
		glDeleteFramebuffers(1, &FBO);
	}

	void ShadowRenderBuffer::MakeCurrent()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, Settings::Video::ShadowResolution, Settings::Video::ShadowResolution);
	}

	void ShadowRenderBuffer::Rebuild()
	{
		DepthTexture.CreateTexture(true, Settings::Video::ShadowResolution, Settings::Video::ShadowResolution);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture.GetID(), 0);
		glDrawBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Debug::Error("ShadowRenderBuffer incomplete.");
		}

		Debug::GLError("ERROR: GLError on ShadowRenderBuffer Rebuild.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowRenderBuffer::Clear()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

}