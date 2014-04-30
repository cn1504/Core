#pragma once
#include "Core.h"

namespace Core
{

	class RenderBuffer
	{
	private:
		GLuint FBO;
		std::vector<GLuint> ColorTextures; 
		bool HasDepthTexture;
		GLuint DepthTexture;
		glm::vec3 ClearColor;

		void CreateTexture(GLuint & into, bool depth, int width, int height);

	public:
		RenderBuffer(glm::vec3 clearColor, int colorBufferCount, bool hasDepthTexture);
		~RenderBuffer();

		void MakeCurrent();
		void Rebuild();
		void Clear();

		GLuint GetOutputTexture(int i);
		GLuint GetDepthTexture();
	};

}
