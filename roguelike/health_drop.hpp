#pragma once

#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "collectible.hpp"

/**
 * @brief      Class for Collectible Health Drops.
 */
class HealthDrop : public Collectible {
private:
	/**
	 * @brief      Amount of health to restore
	 */
	float health = 1.0f;
	
protected:

public:
	/**
	 * @brief      Create a new HealthDrop
	 *
	 * @param      _manager    The global GameManager pointer
	 * @param      parent      The parent GameEntity node
	 * @param[in]  pos         The Position of the HealthDrop
	 * @param[in]  powerLevel  The power level
	 */
	HealthDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, float powerLevel);

	/**
	 * @brief      Draw this to the screen
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Collect this Drop
	 */
	virtual void getCollected() override;
};