#pragma once

#include "Core.h"
#include "Font.h"
#include "Shader.h"

namespace Core
{
	
	class Text
	{
	private:
		std::string Txt;
		Font* Font;
		glm::vec2 Position;
		glm::vec4 Color;
		GLuint VertexBuffer;

		void GenerateGeometry();

	public:
		Text(std::string txt, int x, int y, Core::Font* font, glm::vec4 color = glm::vec4(1.0,1.0,1.0,1.0));
		~Text();

		void UpdateText(std::string txt);

		void Render(Shader* shader);
	};

}