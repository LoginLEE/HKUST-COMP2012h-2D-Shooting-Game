#pragma once

#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "collision_box.hpp"
#include "game_entity.hpp"
#include "utils.hpp"

/**
 * @brief      Class for game manager.
 */
class GameManager;

/**
 * Base class for Entities with Collision Physics
 */ 
class CollidingEntity : public GameEntity {
private:

	/**
	 * @brief      Automatically update the collisionBox field based on world position and size
	 */
	void updateCollisionBox() {setCollisionBox(CollisionBox(getTotalPosition(), getSize()));}
	
protected:
	
	/**
	 * The Collision Box for this Entity
	 */
	CollisionBox collisionBox;

	/**
	 * @brief      Basic Construction-time logic for CollidingEntities
	 *
	 * @param      manager   The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  u         Update priority (default: 0)
	 * @param[in]  d         Draw priority (default: -1)
	 */
	CollidingEntity(GameManager* _manager, GameEntity *parent = nullptr, int u = 0, int d = -1) : GameEntity(_manager, parent, u, d) {}
	
	
public:
	
	/**
	 * @brief      Sets the Collision Box.
	 *
	 * @param[in]  cbox  The new Collision Box
	 */
	void setCollisionBox(CollisionBox cbox) {collisionBox = cbox;}
	
	/**
	 * @brief      Gets the Collision Box.
	 *
	 * @return     The Collision Box.
	 */
	CollisionBox getCollisionBox() const {return collisionBox;}
	
	/**
	 * @brief      Checks for collision with another CollidingEntity
	 *
	 * @param[in]  other  The other Colliding Entity
	 *
	 * @return     true/false: this Entity is currently colliding with the other Entity
	 */
	bool collidesWith(const CollidingEntity &other) const {return collisionBox.checkIntersection(other.collisionBox);}
	
	/**
	 * @brief      Checks for collision with a Collision Box
	 *
	 * @param[in]  cbox  The Collision Box
	 *
	 * @return     true/false: this Entity is currently colliding with the given Collision Box
	 */
	bool collidesWith(const CollisionBox &cbox) const {return collisionBox.checkIntersection(cbox);}
	
	/**
	 * @brief      Sets the position of this Entity relative to its parent
	 *
	 * @param[in]  pos   The relative position
	 */
	virtual void setPosition(sf::Vector2f pos) {GameEntity::setPosition(pos); updateCollisionBox();}
	
	/**
	 * @brief      Sets the position of this Entity relative to its parent
	 *
	 * @param[in]  x     X coordinate of position
	 * @param[in]  y     Y coordinate of position
	 */
	virtual void setPosition(float x, float y) {setPosition(sf::Vector2f(x, y));}
	
	/**
	 * @brief      Sets the Size of this Entity
	 *
	 * @param[in]  sz    The size
	 */
	virtual void setSize(sf::Vector2f sz) {GameEntity::setSize(sz); updateCollisionBox();}
	
	/**
	 * @brief      Sets the size of this Entity
	 *
	 * @param[in]  x     X coordinate of size
	 * @param[in]  y     Y coordinate of size
	 */
	virtual void setSize(float x, float y) {setSize(sf::Vector2f(x, y));}
	
	/**
	 * @brief      Process Simple Physics for this Entity
	 * 
	 * @detailed	Calculates Gravity and Friction
	 *
	 * @param[in]  cboxes  Vector of Collision Boxes representing all Solid Blocks in the world
	 */
	void processBasicPhysics(const std::vector<CollisionBox> &cboxes) {
		// Dirty friction, fix later
		setVelocity(sf::Vector2f(getVelocity().x * 0.95f, getVelocity().y + getGravity()));
		
		// Everything here is local coordinates, but collisionboxes are always updated according to global coordinates
		sf::Vector2f potentialPosition = getPosition() + getVelocity() + getExternalImpact();
		resetExternalImpact();
		
		// Doing this in 2 parts is needed so that ground is not sticky
	
		// Part 1: Move horizontally as much as you can (binary search)
		bool fullHorizontalMove = moveMaximally(sf::Vector2f(potentialPosition.x, getPosition().y), cboxes);
	
		// Part 2: Try moving vertically (with posiibly modified potentialPosition)
		bool fullVerticalMove = moveMaximally(sf::Vector2f(getPosition().x, potentialPosition.y), cboxes);
		if (!fullVerticalMove) {
			setVelocity(getVelocity().x, 0.0f);
		}
	
		// Update collisionBox (in case)
		setCollisionBox(CollisionBox(getTotalPosition(), getSize()));
	}
	
	
	/**
	 * @brief      Process Bouncy Physics for this Entity
	 * 
	 * @detailed	Calculates Gravity, Friction and Bounciness
	 *
	 * @param[in]  cboxes  Vector of Collision Boxes representing all Solid Blocks in the world
	 */
	void processBouncyPhysics(const std::vector<CollisionBox> &cboxes) {
		// Similar to processBasicPhysics
		setVelocity(sf::Vector2f(getVelocity().x * 0.95f, getVelocity().y + getGravity()));
		sf::Vector2f potentialPosition = getPosition() + getVelocity() + getExternalImpact();
		resetExternalImpact();

		bool fullHorizontalMove = moveMaximally(sf::Vector2f(potentialPosition.x, getPosition().y), cboxes);
		if (!fullHorizontalMove) setVelocity( -0.9f * getVelocity().x , getVelocity().y);
		
		bool fullVerticalMove = moveMaximally(sf::Vector2f(getPosition().x, potentialPosition.y), cboxes);
		if (!fullVerticalMove) setVelocity(getVelocity().x, -0.6f * getVelocity().y);
		
		setCollisionBox(CollisionBox(getTotalPosition(), getSize()));
	}
	
	/**
	 * @brief      Simulate instability, by assigning a velocity of given magnitude and random direction
	 *
	 * @param[in]  magnitude  The magnitude of the random velocity
	 */
	void destabilize(float magnitude) {
		setVelocity(utils::normalizedRandomv2() * magnitude);
	}
	
	/**
	 * @brief      Moves in "dest" direction as much as possible
	 *
	 * @param[in]  dest    The destination point
	 * @param[in]  cboxes  Vector of Collision Boxes representing all Solid Blocks in the world
	 *
	 * @return     true if destination was reached without obstruction.
	 */
	bool moveMaximally(sf::Vector2f dest, const std::vector<CollisionBox> &cboxes) {
		bool noObstruction = true;
		sf::Vector2f begin = getPosition();
		sf::Vector2f end = dest;
		while ((end-begin).x * (end-begin).x + (end-begin).y * (end-begin).y > 1.0f) {
			sf::Vector2f mid = (begin + end) / 2.0f;
			//cerr << "begin: " << begin.x << " " << begin.y << endl;
			//cerr << "end: " << end.x << " " << end.y << endl;
			//cerr << "mid: " << mid.x << " " << mid.y << endl;
			setPosition(mid);
			setCollisionBox(CollisionBox(getTotalPosition(), getSize()));
			bool obstruction = checkObstruction(cboxes);
			if (obstruction) {
				noObstruction = false;
				end = mid;
			} else {
				begin = mid;
			}
		}
		setPosition(begin);
		return noObstruction;
	}
	
	/**
	 * @brief      Does collision checking against given collision boxes.
	 *
	 * @param[in]  cboxes  Vector of Collision Boxes representing all Solid Blocks in the world
	 *
	 * @return     true if there was any collision/obstruction
	 */
	bool checkObstruction(const std::vector<CollisionBox> &cboxes) const {
		bool ret = false;
		for (const CollisionBox &cbox : cboxes) {
			if (cbox.checkIntersection(collisionBox)) {
				ret = true;
				break;
			}
		}
		return ret;
	}
	
};