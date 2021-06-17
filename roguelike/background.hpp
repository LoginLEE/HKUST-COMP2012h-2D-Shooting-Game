#pragma once

#include <SFML/Graphics.hpp>
#include "game_entity.hpp"

/**
 * @brief      Class for the Background image of the Level.
 */
class Background : public GameEntity {
private:

protected:
	/**
	 * The Texture/Image for the Background
	 */
	sf::Texture texture;

public:

	/**
	 * @brief      Construct a new Background entity at the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Background(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Construct a new Background entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	Background(GameManager *_manager, GameEntity *parent, float x, float y);
	
	/**
	 * @brief      Construct a new Background entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Background(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);

	/**
	 * @brief      Draw the Background onto the given RenderTarget
	 *
	 * @param      renderer  The RenderTarget to use to draw
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;

};