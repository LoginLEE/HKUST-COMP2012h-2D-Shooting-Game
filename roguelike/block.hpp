#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "colliding_entity.hpp"

/**
 * @brief      Base Class for all Block type Entities
 * 
 * @detailed	This class provides a base type to all Block type Entities
 * 				This means any object that should be considered solid and part of the level environment
 * 				It extends from CollidingEntity which gives the base for handling physics and collisions with Characters
 */
class Block : public CollidingEntity {
private:

protected:

	/**
	 * @brief      Perform the basic construction time logic for all Blocks, at the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Block(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Perform the basic construction time logic for all Blocks, at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	Block(GameManager *_manager, GameEntity *parent, float x, float y);
	
	/**
	 * @brief      Perform the basic construction time logic for all Blocks, at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Block(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);

public:

	/**
	 * @brief      Static helper function to get the SFML Vector Coordinates to a certain block position in the grid
	 *
	 * @param[in]  x     Grid x coordinate
	 * @param[in]  y     Grid y coordinate
	 *
	 * @return     SFML Vector Coordinates for the given Grid position
	 */
	static sf::Vector2f gridCoord(float x, float y) {
		return sf::Vector2f(x, y)*BLOCK_SIZE;
	}
};