#pragma once

#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "collectible.hpp"
#include "gun.hpp"

/**
 * @brief      Class for Collectible Drops containing new Guns
 */
class GunDrop : public Collectible {
private:
	/**
	 * @brief      The Contained Gun
	 */
	Gun *gun = nullptr;
	
	/**
	 * @brief      Excitement level
	 */
	int excitement = 0;
	
protected:
	/**
	 * @brief      Texture of the Gun Drop
	 */
	sf::Texture texture;

public:

	/**
	 * @brief      Create a new Gun Drop containing the given Gun
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The Position of the GunDrop
	 * @param      g         The Gun to contain
	 */
	GunDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, Gun *g);

	/**
	 * @brief      Create a new Gun Drop containing a random Gun of given powerLevel
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The Position of the GunDrop
	 * @param[in]  powerLevel  The power level
	 */
	GunDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, float powerLevel);

	/**
	 * @brief      Update function for this Entity
	 */
	virtual void update() override;
	
	/**
	 * @brief      Draw function for this Entity
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Collect this Gun Drop
	 */
	virtual void getCollected() override;
	
	/**
	 * @brief      Excite this entity
	 *
	 * @param[in]  x     Excitement level
	 */
	void excite(int x) {excitement = std::max(excitement, x);}
	
	/**
	 * @brief      Gets the gun.
	 *
	 * @return     The gun.
	 */
	Gun* getGun() const {return gun;}
};