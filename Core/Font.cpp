#include "Font.h"

namespace Core
{

	Font::Font(Core::Texture* texture, float charWidth, float charHeight, int charsPerRow, float spacingWidth, int spacingHeight)
	{
		Texture = texture;
		CharWidth = charWidth;
		CharHeight = charsPerRow;
		CharsPerRow = charsPerRow;
		SpacingWidth = spacingWidth;
		SpacingHeight = spacingHeight;
	}


	Font::~Font()
	{
	}

}