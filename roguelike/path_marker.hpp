#pragma once

#include <SFML/Graphics.hpp>
#include "global_defines.hpp"
#include "game_entity.hpp"
#include "collision_box.hpp"
#include "textured_block.hpp"

class Enemy;
/**
 * @brief      Class for Enemy Path Markers
 * 
 * This class represents a invisible Entity that guides the Enemy and controls their pathing around the level
 */
class PathMarker : public GameEntity {
public:
	/**
	 * @brief      Movement Direction for the Enemy
	 */
	enum Direction {
		EnemyPathIdle,
		EnemyPathLeft, 
		EnemyPathRight,
		EnemyPathJumpLeft,
		EnemyPathJumpRight,
	};

private:
	/**
	 * @brief      The movement direction that this path marker will tell an Enemy to follow
	 */
	Direction pathDir = Direction::EnemyPathRight;
	
	/**
	 * @brief      Speed of the path that the enemy should take
	 */
	float speed = 1.0f;
	
	/**
	 * @brief      Should the enemy shoot while following this path?
	 */
	bool shoot = true;
	
	/**
	 * @brief      CollisionBox of this Path Marker
	 */
	CollisionBox collisionBox;
	
	/**
	 * @brief      Create a new Path Marker with a certain generation code at the default position
	 *
	 * @param      _manager  		The global GameManager pointer
	 * @param[in]  generationCode   The generation code
	 * @param      parent    		The parent GameEntity node
	 */
	PathMarker(GameManager *_manager, int generationCode, GameEntity *parent)
		: PathMarker(_manager, generationCode, parent, 0, 0) {}
	
	/**
	 * @brief      Create a new Path Marker with a certain generation code at the given position
	 *
	 * @param      _manager  		The global GameManager pointer
	 * @param[in]  generationCode  	The generation code
	 * @param      parent    		The parent GameEntity node
	 * @param[in]  x         		x coordinate
	 * @param[in]  y        		y coordinate
	 */
	PathMarker(GameManager *_manager, int generationCode, GameEntity *parent, float x, float y)
		: PathMarker(_manager, generationCode, parent, sf::Vector2f(x, y)) {}
	
	/**
	 * @brief      Create a new Path Marker with a certain generation code at the given position
	 *
	 * @param      _manager  		The global GameManager pointer
	 * @param[in]  generationCode  	The generation code
	 * @param      parent    		The parent GameEntity node
	 * @param[in]  pos             	The position
	 */
	PathMarker(GameManager *_manager, int generationCode, GameEntity *parent, sf::Vector2f pos)
		: GameEntity(_manager, parent) {

		pathDir = static_cast<Direction>(generationCode % 5);
		
		setPosition(pos);
		setSize(sf::Vector2f(BLOCK_SIZE*0.5f, BLOCK_SIZE));
		switch (pathDir) {
			case EnemyPathIdle:
				setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
				break;
			case EnemyPathLeft:
			case EnemyPathJumpLeft:
				setPosition(pos + sf::Vector2f(BLOCK_SIZE*0.25f, 0));
				break;
			case EnemyPathRight:
			case EnemyPathJumpRight:
				setPosition(pos - sf::Vector2f(BLOCK_SIZE*0.25f, 0));
				break;
		}
		collisionBox = CollisionBox(getTotalPosition(), getSize());
	}

public:

	/**
	 * @brief      Static function for creating Path Markers from Map file codes
	 * 
	 * Used to allow for the possibility of generating subclasses of Path Markers in the future
	 *
	 * @param      _manager  		The global GameManager pointer
	 * @param[in]  generationCode  	The generation code
	 * @param      parent    		The parent GameEntity node
	 * @param[in]  x               	The X coordinate
	 * @param[in]  y               	The Y coordinate
	 *
	 * @return     The created Path Marker
	 */
	static PathMarker* create(GameManager *_manager, int generationCode, GameEntity *parent, float x, float y) {
		return new PathMarker(_manager, generationCode, parent, x, y); //may return sub classes in the future
	}

	/**
	 * @brief      Gets the collision box.
	 *
	 * @return     The collision box.
	 */
	CollisionBox getCollisionBox() const {return collisionBox;}

	/**
	 * @brief      Is the given Enemy Entity stepping in this Path Marker?
	 *
	 * @param[in]  enemy  The enemy
	 *
	 * @return     True if Enemy's feet is inside this collision box
	 */
	bool steppingIn(const Enemy* enemy) const;

	/**
	 * @brief      Gets the path direction
	 */
	Direction getPathDir() const {return pathDir;};
	
	/**
	 * @brief      Gets the speed.
	 */
	float getSpeed() const {return speed;};
	
	/**
	 * @brief      Should enemy shoot?
	 *
	 * @return     { description_of_the_return_value }
	 */
	float shouldShoot() const {return shoot;};

	/**
	 * @brief      Draw Collision Box and Arrows for Debugging
	 *
	 * @param      renderer  The Render Target to Draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override {
		if (DISPLAY_DEBUGGING_STUFF) {
			collisionBox.draw(renderer);
			if (pathDir == EnemyPathIdle) return;
			sf::ConvexShape arrow;
			arrow.setPointCount(3);
			switch (pathDir) {
				case EnemyPathLeft:
					arrow.setPoint(0, sf::Vector2f(getSize().x, 0));
					arrow.setPoint(1, sf::Vector2f(0, getSize().y/2));
					arrow.setPoint(2, sf::Vector2f(getSize().x, getSize().y));
					break;
				case EnemyPathRight:
					arrow.setPoint(0, sf::Vector2f(0, 0));
					arrow.setPoint(1, sf::Vector2f(0, getSize().y));
					arrow.setPoint(2, sf::Vector2f(getSize().x, getSize().y/2));
					break;
				case EnemyPathJumpLeft:
					arrow.setPoint(0, sf::Vector2f(0, 0));
					arrow.setPoint(1, sf::Vector2f(0, getSize().y/2));
					arrow.setPoint(2, sf::Vector2f(getSize().x, 0));
					break;
				case EnemyPathJumpRight:
					arrow.setPoint(0, sf::Vector2f(0, 0));
					arrow.setPoint(1, sf::Vector2f(getSize().x, getSize().y/2));
					arrow.setPoint(2, sf::Vector2f(getSize().x, 0));
					break;
				case EnemyPathIdle:
					break;
			}
			arrow.setFillColor(sf::Color::Green);
			arrow.setPosition(getTotalPosition());
			arrow.setOrigin(getSize()/2.0f);
			renderer.draw(arrow);
		}
	}
};