#pragma once
#include "Core.h"
#include "Texture.h"

namespace Core
{

	class RenderBuffer
	{
	private:
		GLuint FBO;
		std::vector<Texture> ColorTextures; 
		bool HasDepthTexture;
		Texture DepthTexture;
		glm::vec4 ClearColor;

	public:
		RenderBuffer(glm::vec4 clearColor, int colorBufferCount, bool hasDepthTexture);
		~RenderBuffer();

		void MakeCurrent();
		void Rebuild();
		void Clear();

		GLuint GetOutputTexture(int i);
		GLuint GetDepthTexture();
	};

}
