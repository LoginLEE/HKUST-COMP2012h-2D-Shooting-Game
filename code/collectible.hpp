#pragma once

#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "colliding_entity.hpp"

/**
 * @brief      Base Class for Collectible Entities
 */
class Collectible : public CollidingEntity {
private:

protected:
	/**
	 * @brief      Handles special physics and collisions for Collectible Entities
	 */
	void processCollectiblePhysics();

public:
	/**
	 * @brief      Perform the basic construction time logic for all Characters, at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Collectible(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);
	
	/**
	 * @brief      Basic draw function for Collectibles
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Basic update function for Collectibles
	 */
	virtual void update() override;
	
	/**
	 * @brief      Pure virtual function for collecting this Drop
	 */
	virtual void getCollected() = 0;
	
	/**
	 * @brief      Creates a Random Collectible at the given position, with given power level
	 *
	 * @param      manager   The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 * @param[in]  powerLevel  The power level for the new collectible
	 *
	 * @return     A randomly generated Collectible
	 */
	static Collectible* createCollectibleWithPower(GameManager *manager, GameEntity *parent, sf::Vector2f pos, float powerLevel);
};