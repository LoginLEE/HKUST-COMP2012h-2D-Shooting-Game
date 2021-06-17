#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "block.hpp"
class GameLevel;

/**
 * @brief      Class for Teleporter Blocks
 */
class Teleporter : public Block {
private:
	/**
	 * @brief      Pointer to level which this teleporter is inside
	 */
	GameLevel *level = nullptr;
	
	/**
	 * @brief      The associated teleporter this Block is paired to
	 */
	Teleporter *associate = nullptr;
	
	/**
	 * @brief      Timer for teleporter cooldown
	 */
	int inactiveUntil = 0;

protected:
	
	/**
	 * @brief      The texture for this Teleporter Block
	 */
	sf::Texture texture;

public:
	
	/**
	 * @brief      Types of Teleporters
	 */
	enum Type {
		PlayerSpawn, UpTeleporter, DownTeleporter, LeftTeleporter, RightTeleporter//, ...
	};

	/**
	 * @brief      Construct a teleporter block in the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Teleporter(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Construct a teleporter block in the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	Teleporter(GameManager *_manager, GameEntity *parent, float x, float y);
	
	/**
	 * @brief      Construct a teleporter block in the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Teleporter(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);
	
	/**
	 * @brief      Determines if active.
	 *
	 * @return     True if active, False otherwise.
	 */
	bool isActive() const {return level && associate;}
	
	/**
	 * @brief      Gets the associated teleport block
	 */
	Teleporter* getAssociate() const {return associate;}
	
	/**
	 * @brief      Gets the residing level.
	 */
	GameLevel* getLevel() const {return level;}
	
	/**
	 * @brief      Sets the residing level.
	 */
	void setLevel(GameLevel *lvl) {level = lvl;}
	
	/**
	 * @brief      Sets the associated teleporter block
	 */
	void setAssociate(Teleporter *assoc) {associate = assoc;}
	
	/**
	 * @brief      Deactivates the teleporter and starts the cooldown timer
	 */
	void overheat() {inactiveUntil = CURRENT_TICK + 60;}
	
	/**
	 * @brief      Update function for the Teleporter Block
	 */
	virtual void update() override;
	
	/**
	 * @brief      Draw function for the Teleporter Block
	 *
	 * @param      renderer  The RenderTarget to render to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
};