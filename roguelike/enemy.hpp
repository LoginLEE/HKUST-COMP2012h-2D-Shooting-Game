#pragma once

#include <SFML/Graphics.hpp>
#include "global_defines.hpp"
#include "character.hpp"

class GameLevel;
class Gun;
class PathMarker;

/**
 * @brief      Class for Enemy Entities
 */
class Enemy : public Character {
private:

protected:
	/**
	 * @brief The Character to target
	 */
	Character *target = nullptr;
	
	/**
	 * @brief This enemies Gun
	 */
	Gun* gun = nullptr;
	
	/**
	 * Current Path Marker that this Enemy's AI uses to decide movement
	 */
	PathMarker* currentPath = nullptr;
	
	/**
	 * @brief      "How much" the target is found
	 */
	float foundTargetCounter = 0.0f;
	
public:
	
	/**
	 * @brief      Construct an Enemy Entity at the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Enemy(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Construct an Enemy Entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	Enemy(GameManager *_manager, GameEntity *parent, float x, float y);
	
	/**
	 * @brief      Construct an Enemy Entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Enemy(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);

	/**
	 * @brief      Gets the current targeted Character.
	 */
	Character* getTarget();
	
	/**
	 * @brief      Sets the current targeted Character.
	 */
	void setTarget(Character *tgt);
	
	/**
	 * @brief      Draw this Enemy to the screen
	 *
	 *	Handles textures and animation
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Enemy Physics Handling and AI logic
	 */
	virtual void update() override;

	/**
	 * @brief      Gets the feet position.
	 */
	sf::Vector2f getFeetPos() const {
		return getTotalPosition() + sf::Vector2f(0, getSize().y*0.25f);
	}
};
