#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "colliding_entity.hpp"
#include "damage.hpp"
#include "overhead_bar.hpp"

class GameLevel;
class GameManager;
class Block;

/**
 * @brief      Enum to represent the different kinds of Character Motion and
 *             also the frames of the Character animation
 */
enum CHARACTER_MOTION
{
	stand_left = 0,
	stand_right,
	move_left1,
	move_left2,
	move_left3,
	move_left4,
	move_left5,
	move_left6,
	move_left7,
	move_left8,
	move_right1,
	move_right2,
	move_right3,
	move_right4,
	move_right5,
	move_right6,
	move_right7,
	move_right8,
	empty1
};

/**
 * @brief      Class for Character Type Entities
 * 
 * @detailed	Provides Basic Common Logic and Attributes for Character Type Entities (such as Player and Enemies)
 */
class Character : public CollidingEntity {
protected:
	
	/**
	 * @brief The current health amount of this Character
	 */
	float health = 0.0f;

	/**
	 * @brief The maximum health amount of this Character
	 */
	float maxHealth = 0.0f;
	
	/**
	 * @brief The Health Bar for this Character
	 */
	OverheadBar<float> *healthBar;

	/**
	 * @brief Pointer to textures for this Character
	 */
	sf::Texture *texture;

	/**
	 * @brief Current motion of this Character
	 */
	CHARACTER_MOTION current_motion = stand_right;

	/**
	 * @brief      Perform the basic construction time logic for all Characters, at the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Character(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Perform the basic construction time logic for all Blocks, at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Character(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);
	
	
	/**
	 * @brief      Perform the basic construction time logic for all Characters, at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	Character(GameManager *_manager, GameEntity *parent, float x, float y);

public:

	/**
	 * @brief      Tries to subtract a given amount of ammo from this Character for reloading their gun
	 *
	 * @param[in]  reload  The reload amount
	 *
	 * @return     How much ammo was able to be reloaded
	 */
	virtual int subtractAmmo(int reload) {return reload;};
	
	/**
	 * @brief      Apply the given amount and type of Damage to the Character
	 *
	 * @param[in]  damage  The Damage type and amount
	 */
	void takeDamage(Damage damage);
	
	/**
	 * @brief      Returns the Characters current health
	 */
	float getHealth() const {return health;}

	/**
	 * @brief      Returns the Characters maximum health
	 */
	float getMaxHealth() const {return maxHealth;}

};

