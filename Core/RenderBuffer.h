#pragma once
#include "Core.h"
#include "Texture.h"

namespace Core
{

	class RenderBuffer
	{
	protected:
		GLuint FBO;
		std::vector<Texture> ColorTextures; 
		bool HasDepthTexture;
		Texture DepthTexture;
		glm::vec4 ClearColor;

	public:
		RenderBuffer(glm::vec4 clearColor, int colorBufferCount, bool hasDepthTexture);
		virtual ~RenderBuffer();

		virtual void MakeCurrent();
		virtual void Rebuild();
		virtual void Clear();

		GLuint GetOutputTexture(int i);
		GLuint GetDepthTexture();
	};


	class ShadowRenderBuffer : public RenderBuffer
	{
	public:
		ShadowRenderBuffer();
		virtual ~ShadowRenderBuffer();

		virtual void MakeCurrent();
		virtual void Rebuild();
		virtual void Clear();
	};

}
