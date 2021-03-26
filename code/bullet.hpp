#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "colliding_entity.hpp"
#include "damage.hpp"
#include "utils.hpp"

class GameLevel;
class GameManager;

/**
 * @brief      Class for Bullet Entities
 * 
 * @detailed	This class is used for Objects that represent travelling Entities that damage Characters they collide with
 * 				Extends from CollidingEntity to use the Collision Handling logic
 */
class Bullet : public CollidingEntity {
protected:
	/**
	 * @brief Descriptor of amount and type of Damage to inflict
	 */
	Damage damage;
	
	/**
	 * @brief Time in update cycles that this Bullet can live before dissipating
	 */
	int timeToLive = 30;
	
public:
	/**
	 * @brief      Constructs a Bullet Entity at the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Bullet(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Constructs a Bullet Entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	Bullet(GameManager *_manager, GameEntity *parent, float x, float y);
	
	/**
	 * @brief      Constructs a Bullet Entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Bullet(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);
	
	/**
	 * @brief      Gets the Damage Type and Amount for this Bullet Entity.
	 *
	 * @return     The Damage Type and Amount for this Bullet Entity.
	 */
	Damage getDamage() const;
	
	/**
	 * @brief      Sets the Damage Type and Amount for this Bullet Entity.
	 *
	 * @param[in]  dmg   The Damage Type and Amount for this Bullet Entity.
	 */
	void setDamage(Damage dmg);
	
	/**
	 * @brief      Sets the time to live for this Bullet Entity.
	 *
	 * @param[in]  ttl   The time to live
	 */
	void setTimeToLive(int ttl);
	
	
	/**
	 * @brief      Draw this object onto the given RenderTarget
	 *
	 * @param      renderer  The RenderTarget to use to draw
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Update function for this Bullet Entity
	 * 
	 * @detailed	Updates the various properties of this Bullet
	 * 				Handles Time to Live and Damaging Characters
	 */
	virtual void update() override;
};

