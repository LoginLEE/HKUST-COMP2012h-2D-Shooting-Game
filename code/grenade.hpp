#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "colliding_entity.hpp"
#include "damage.hpp"

class GameManager;
class CollisionRay;

/**
 * @brief      Class for Grenade Entities.
 */
class Grenade : public CollidingEntity {
private:
	
protected:
	
	/**
	 * @brief      Type and Amount of Damage to deal
	 */
	Damage dmg;
	
	/**
	 * @brief      Number of bullets to shoot
	 */
	int num = 18; 
	
	/**
	 * @brief      Bullet speed
	 */
	float spd = 200.0f; 
	
	/**
	 * @brief      Bullets Time to Live
	 */
	int ttl = 3; 
	
	/**
	 * @brief      Wait Time before the Grenade explodes
	 */
	int waitTime = 120; 
	
	/**
	 * @brief      The texture of the Grenade
	 */
	sf::Texture *texture;
				
public:
	/**
	 * @brief      Construct a new Grenade
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Grenade(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Game Update for this Grenade
	 */
	virtual void update();
	
	/**
	 * @brief      Draw this Grenade
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const;
};

