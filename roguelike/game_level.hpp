#pragma once

#include <algorithm>
#include <vector>

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "game_entity.hpp"
#include "game_entity.hpp"
#include "block.hpp"
#include "colored_block.hpp"
#include "textured_block.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "teleporter.hpp"
#include "background.hpp"
#include "path_marker.hpp"

class GameManager;

/**
 * @brief      Class for Game Level
 * 
 * GameLevels also extend from GameEntity, and should always act as the Root Entity Node
 */
class GameLevel : public GameEntity {
private:

	/**
	 * @brief      Difficulty of this level
	 */
	float difficulty;
	
	/**
	 * @brief      Active Render Window
	 */
	sf::RenderWindow &renderWindow;
	/**
	 * @brief      Active SFML View
	 */
	sf::View &view;
	/**
	 * @brief      The Level to the Left of this Level
	 */
	GameLevel *leftLevel = nullptr;
	/**
	 * @brief      The Level to the Right of this Level
	 */
	GameLevel *rightLevel = nullptr;
	/**
	 * @brief      The Level to the Up of this Level
	 */
	GameLevel *upLevel = nullptr;
	/**
	 * @brief      The Level to the Down of this Level
	 */
	GameLevel *downLevel = nullptr;
	/**
	 * @brief      The Teleporter for the Left Level
	 */
	Teleporter *leftTeleporter = nullptr;
	/**
	 * @brief      The Teleporter for the Right Level
	 */
	Teleporter *rightTeleporter = nullptr;
	/**
	 * @brief      The Teleporter for the Upper Level
	 */
	Teleporter *upTeleporter = nullptr;
	/**
	 * @brief      The Teleporter for the Lower Level
	 */
	Teleporter *downTeleporter = nullptr;
	/**
	 * @brief      The Current Cursor Position
	 */
	sf::Vector2f cursorPosition;
	/**
	 * @brief      Pointer to the active player character
	 */
	Player *player;
	
public:
	
	/**
	 * @brief      Construct a new Game Level
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      difficulty    The difficulty of the level
	 * @param      renderWindow  The Render Window
	 * @param      view          The View
	 */
	GameLevel(GameManager *_manager, float difficulty, sf::RenderWindow &renderWindow, sf::View &view);
	
	/**
	 * @brief      Destroys the object.
	 */
	~GameLevel();
	
	/**
 	* @brief      Gets the difficulty of the level
	*/
	float getDifficulty() const;

	/**
	 * @brief      Gets the left level.
	 */
	GameLevel* getLeftLevel();
	
	/**
	 * @brief      Gets the right level.
	 */
	GameLevel* getRightLevel();
	
	/**
	 * @brief      Gets the up level
	 */
	GameLevel* getUpLevel();
	
	/**
	 * @brief      Gets the down level
	 */
	GameLevel* getDownLevel();
	
	/**
	 * @brief      Sets the left level.
	 */
	void setLeftLevel(GameLevel *lvl);
	
	/**
	 * @brief      Sets the right level.
	 */
	void setRightLevel(GameLevel *lvl);
	
	/**
	 * @brief      Sets the up level
	 */
	void setUpLevel(GameLevel *lvl);
	
	/**
	 * @brief      Sets the down level
	 */
	void setDownLevel(GameLevel *lvl);
	
	/**
	 * @brief      Gets the left teleporter.
	 */
	Teleporter* getLeftTeleporter();
	
	/**
	 * @brief      Gets the right teleporter.
	 */
	Teleporter* getRightTeleporter();
	
	/**
	 * @brief      Gets the up teleporter
	 */
	Teleporter* getUpTeleporter();
	
	/**
	 * @brief      Gets the down teleporter
	 */
	Teleporter* getDownTeleporter();
	
	/**
	 * @brief      Sets the left teleporter.
	 */
	void setLeftTeleporter(Teleporter *tp);
	
	/**
	 * @brief      Sets the right teleporter.
	 */
	void setRightTeleporter(Teleporter *tp);
	
	/**
	 * @brief      Sets the up teleporter
	 */
	void setUpTeleporter(Teleporter *tp);
	
	/**
	 * @brief      Sets the down teleporter
	 */
	void setDownTeleporter(Teleporter *tp);
	
	/**
	 * @brief      Gets the cursor position.
	 */
	sf::Vector2f getCursorPosition() const;
	
	/**
	 * @brief      Sets the active player in this level.
	 */
	void setPlayer(Player *p);
	
	/**
	 * @brief      Adds an entity to the level
	 */
	void addEntity(GameEntity *entity);
	
	/**
	 * @brief      Gets all entities in the level.
	 */
	std::vector<const GameEntity*> getEntities() const;
	
	/**
	 * @brief      Gets all entities in the level.
	 */
	std::vector<GameEntity*> getEntities();
	
	/**
	 * @brief      Gets all block collision boxes in the level
	 */
	std::vector<CollisionBox> getBlockCollisionBoxes() const;
	
	/**
	 * @brief      Gets all entities colliding with the given collision box in the level
	 *
	 * @param[in]  cbox  The collision box to check
	 *
	 * @return     All entities currently colliding with the given collision box
	 */
	std::vector<CollidingEntity*> getCollidingEntities(CollisionBox cbox);
	
	/**
	 * @brief      Update function for the Game Level
	 * 
	 * Updates cursor position and camera position
	 */
	void update();
};