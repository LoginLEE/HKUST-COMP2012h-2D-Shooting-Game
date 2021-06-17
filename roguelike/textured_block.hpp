#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "colliding_entity.hpp"
#include "block.hpp"
#include "game_manager.hpp"

/**
 * @brief      Types of Textured Blocks
 */
enum BLOCK_TYPE
{
	GRASS = 0,
	SOIL,
	BOX
};

/**
 * @brief      Class for Simple Textured Blocks.
 */
class TexturedBlock : public Block {
private:

protected:
	/**
	 * @brief      The texture for this block
	 */
	sf::Texture texture;
	/**
	 * @brief      The type of this block
	 */
	BLOCK_TYPE block_type;

public:
	
	/**
	 * @brief      Construct a textured block of the given type at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param[in]  type      The block type
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	TexturedBlock(GameManager *_manager, BLOCK_TYPE type, GameEntity *parent, float x, float y)
		: TexturedBlock(_manager, type, parent, sf::Vector2f(x, y)) {};
	
	/**
	 * @brief      Construct a textured block of the given type at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param[in]  type      The block type
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	TexturedBlock(GameManager *_manager, BLOCK_TYPE type, GameEntity *parent, sf::Vector2f pos)
		: Block(_manager, parent, pos), block_type(type) {
		loadTexture();
	}

	/**
	 * @brief      Loads the texture of this block
	 */
	void loadTexture();
	
	/**
	 * @brief      Sets the type of this block
	 */
	void setType(BLOCK_TYPE type) { block_type = type; loadTexture();};
	
	/**
	 * @brief      Gets the type of this block
	 */
	BLOCK_TYPE getType() {return block_type;};
	
	/**
	 * @brief      Draw function for textured blocks
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override {
		sf::Sprite sprite(texture);

		sprite.setScale(getSize().x / texture.getSize().x, getSize().y / texture.getSize().y);
		sprite.setPosition(getTotalPosition());
		sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f));
		renderer.draw(sprite);
	}
};