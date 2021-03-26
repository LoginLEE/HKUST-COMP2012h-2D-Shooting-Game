#include "textured_block.hpp"


void TexturedBlock::loadTexture()
{
	texture = manager->getTexture(BLOCK_TEXTURE, block_type);
}
