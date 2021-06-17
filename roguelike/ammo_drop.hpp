#pragma once

#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "collectible.hpp"

/**
 * @brief      Class for collectible ammo drops.
 * 
 * @detailed	This is the class for Collectible Drops that give the player extra ammo
 * 				It extends from Collectible which provides the basic logic functions for all Collectible Entities
 */
class AmmoDrop : public Collectible {
private:
	/**
	 * @brief Amount of ammo to be given when collected
	 */
	int ammo = 0;
	
protected:
	/**
	 * @brief Texture for this AmmoDrop
	 */
	sf::Texture texture;

public:

	/**
	 * @brief      Construct a new AmmoDrop object
	 * 
	 * @detailed	Constructs a new AmmoDrop object, at the given position, with the ammo given based on the powerLevel parameter
	 *
	 * @param      _manager    The global Game Manager pointer
	 * @param      parent      The parent GameEntity node
	 * @param[in]  pos         The position to create this object at, relative to its parent node
	 * @param[in]  powerLevel  The Power Level parameter, to vary the amount of ammo given
	 */
	AmmoDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, float powerLevel);

	/**
	 * @brief      Draw this object onto the given RenderTarget
	 *
	 * @param      renderer  The RenderTarget to use to draw
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Collect this drop by the player, giving him the ammo contained inside
	 */
	virtual void getCollected() override;
};