#pragma once

#include "Core.h"
#include "Texture.h"
#include "Shader.h"

namespace Core
{
	
	class Text
	{
	private:
		std::string Txt;
		Texture* Font;
		glm::vec2 Position;
		glm::vec4 Color;
		glm::vec2 CharDimensions;
		GLuint VertexBuffer;

		void GenerateGeometry();

	public:
		Text(std::string txt, int x, int y, glm::vec4 color, Texture* font, int charWidth, int charHeight);
		~Text();

		void UpdateText(std::string txt);

		void Render(Shader* shader);
	};

}