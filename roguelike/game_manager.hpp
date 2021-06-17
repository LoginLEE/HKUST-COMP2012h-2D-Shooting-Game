#pragma once

#include <algorithm>

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "game_level.hpp"
#include "player.hpp"
#include "resource_manager.hpp"
#include "hud.hpp"

/**
 * @brief      Class for the Global Game Manager that handles top level Game
 *             Logiv
 */
class GameManager {
private:
	/**
	 * @brief      Active Render Window
	 */
	sf::RenderWindow &renderWindow;
	/**
	 * @brief      Active SFML View
	 */
	sf::View view;
	/**
	 * @brief      vector of all game levels
	 */
	std::vector<GameLevel*> levels;
	/**
	 * @brief      Player Character Entity in this game
	 */
	Player *player;
	
	/**
	 * @brief      Find the nearest entity to the player
	 */
	void updateNearestEntity();
	
	/**
 	* @brief      Nearest of the Selectable Entities (Usually a Collectible)
 	*/
	GameEntity *nearestSelectableEntity = nullptr;

	/**
	 * @brief      Selected Entity (Usually a Collectible)
	 */
	GameEntity *selectedEntity = nullptr;
	/**
	 * @brief      Resource Manager to centralize loading of textures
	 */
	ResourceManager resourceManager;
	
	/**
	 * @brief      Loads all game levels from the Map File
	 *
	 * @param      map_path  The map path
	 */
	void loadFromMap(std::string map_path);

	/**
	 * @brief      HUD
	 */
	HUD *hud;
	
public:

	/**
	 * @brief      Constructs the Game Manager
	 *
	 * @param      renderWindow  The render window
	 * @param[in]  renderWidth   The render width
	 * @param[in]  renderHeight  The render height
	 */
	GameManager(sf::RenderWindow &renderWindow, int renderWidth, int renderHeight);
	
	/**
	 * @brief      Destroys the object.
	 */
	~GameManager();
	
	/**
	 * @brief      Gets the texture.
	 *
	 * @param[in]  type    The type
	 * @param[in]  number  The number
	 *
	 * @return     The texture.
	 */
	sf::Texture getTexture(TXETURE_TYPE type, int number){
		return resourceManager.getTexture(type, number);
	}
	
	/**
	 * @brief      Handler for Processing sf::Events
	 *
	 * @param[in]  event  The event to process
	 */
	void processEvent(const sf::Event &event);
	
	/**
	 * @brief      Start update cycle of the Game
	 */
	void update();
	
	/**
	 * @brief      Draw the game
	 */
	void draw();
	
	/**
	 * @brief      Returns the Player Character Entity
	 */
	Player* getPlayer() const;
	
	/**
	 * @brief      Pointer to the active Game Level
	 */
	GameLevel *activeLevel;

	/**
	 * @brief      Gets the view center position.
	 */
	sf::Vector2f getViewPosition() const {return view.getCenter();}
	/**
	 * @brief      Gets the view size
	 */
	sf::Vector2f getViewSize() const {return view.getSize();}
	
	
	GameEntity* getSelectedEntity() const {return selectedEntity;}


};