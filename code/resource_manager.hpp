#pragma once

#include <algorithm>

#include <SFML/Graphics.hpp>
#include "character.hpp"
#include "global_defines.hpp"
#include "gun.hpp"
#include "textured_block.hpp"

/**
 * @brief      Different Types of Textures, Used to Get the texture from the
 *             Resource Manager
 */
enum TXETURE_TYPE
{
	BACKGROUND_TEXTURE = 0,
	PLAYER_TEXTURE,
	GUN_TEXTURE,
	ENEMY_TEXTURE,
	BLOCK_TEXTURE,
	AMMO_DROP_TEXTURE,
	GUN_DROP_TEXTURE
};


/**
 * @brief      This class manages Loading and Distribution of Textures to all
 *             Entities
 */
class ResourceManager {
private:
	/**
	 * @brief      The array of Textures
	 */
	sf::Texture texture[7][50];

public:
	
	/**
	 * @brief      Constructs the Resource Manager and loads all resources
	 */
	ResourceManager() { /*LoadAllResource();*/ }
	
	/**
	 * @brief      Destroys the object.
	 */
	~ResourceManager() {}
	
	/**
	 * @brief      Gets the texture based on type and index
	 *
	 * @param[in]  type   The type
	 * @param[in]  index  The index
	 *
	 * @return     The texture.
	 */
	const sf::Texture& getTexture(TXETURE_TYPE type, int index) const { return texture[type][index]; }

	/**
	 * @brief      Loads all resources
	 */
	void LoadAllResource()
	{
		//background
		texture[BACKGROUND_TEXTURE][0].loadFromFile(TEXTURE_PATH + "background/background1.png");

		//player
		texture[PLAYER_TEXTURE][0].loadFromFile(TEXTURE_PATH + "player/stl.png");
		texture[PLAYER_TEXTURE][1].loadFromFile(TEXTURE_PATH + "player/str.png");
		texture[PLAYER_TEXTURE][2].loadFromFile(TEXTURE_PATH + "player/ml1.png");
		texture[PLAYER_TEXTURE][3].loadFromFile(TEXTURE_PATH + "player/ml2.png");
		texture[PLAYER_TEXTURE][4].loadFromFile(TEXTURE_PATH + "player/ml3.png");
		texture[PLAYER_TEXTURE][5].loadFromFile(TEXTURE_PATH + "player/ml4.png");
		texture[PLAYER_TEXTURE][6].loadFromFile(TEXTURE_PATH + "player/ml5.png");
		texture[PLAYER_TEXTURE][7].loadFromFile(TEXTURE_PATH + "player/ml6.png");
		texture[PLAYER_TEXTURE][8].loadFromFile(TEXTURE_PATH + "player/ml7.png");
		texture[PLAYER_TEXTURE][9].loadFromFile(TEXTURE_PATH + "player/ml8.png");
		texture[PLAYER_TEXTURE][10].loadFromFile(TEXTURE_PATH + "player/mr1.png");
		texture[PLAYER_TEXTURE][11].loadFromFile(TEXTURE_PATH + "player/mr2.png");
		texture[PLAYER_TEXTURE][12].loadFromFile(TEXTURE_PATH + "player/mr3.png");
		texture[PLAYER_TEXTURE][13].loadFromFile(TEXTURE_PATH + "player/mr4.png");
		texture[PLAYER_TEXTURE][14].loadFromFile(TEXTURE_PATH + "player/mr5.png");
		texture[PLAYER_TEXTURE][15].loadFromFile(TEXTURE_PATH + "player/mr6.png");
		texture[PLAYER_TEXTURE][16].loadFromFile(TEXTURE_PATH + "player/mr7.png");
		texture[PLAYER_TEXTURE][17].loadFromFile(TEXTURE_PATH + "player/mr8.png");

		//gun
		texture[GUN_TEXTURE][0].loadFromFile(TEXTURE_PATH + "gun/pistolL.png");
		texture[GUN_TEXTURE][1].loadFromFile(TEXTURE_PATH + "gun/pistolR.png");
		texture[GUN_TEXTURE][2].loadFromFile(TEXTURE_PATH + "gun/shotgunL.png");
		texture[GUN_TEXTURE][3].loadFromFile(TEXTURE_PATH + "gun/shotgunR.png");
		texture[GUN_TEXTURE][4].loadFromFile(TEXTURE_PATH + "gun/pistol_icon.png");
		texture[GUN_TEXTURE][5].loadFromFile(TEXTURE_PATH + "gun/shotgun_icon.png");

		//enemy
		texture[ENEMY_TEXTURE][0].loadFromFile(TEXTURE_PATH + "enemy/sl.png");
		texture[ENEMY_TEXTURE][1].loadFromFile(TEXTURE_PATH + "enemy/sr.png");

		//block
		texture[BLOCK_TEXTURE][0].loadFromFile(TEXTURE_PATH + "block/grass.png");
		texture[BLOCK_TEXTURE][1].loadFromFile(TEXTURE_PATH + "block/soil.png");
		texture[BLOCK_TEXTURE][2].loadFromFile(TEXTURE_PATH + "block/box.png");

		//ammo drop
		texture[AMMO_DROP_TEXTURE][0].loadFromFile(TEXTURE_PATH + "ammo_drop/ammo1.png");

		//gun drop
		texture[GUN_DROP_TEXTURE][0].loadFromFile(TEXTURE_PATH + "gun_drop/pistol.png");
		texture[GUN_DROP_TEXTURE][1].loadFromFile(TEXTURE_PATH + "gun_drop/shotgun.png");
	}

};