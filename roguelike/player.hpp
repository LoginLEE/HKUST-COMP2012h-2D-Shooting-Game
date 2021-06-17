#pragma once

#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include "character.hpp"
#include "gun.hpp"
#include "collectible.hpp"

/**
 * @brief      Class for the Player Character Entity
 */
class Player : public Character {
private:
	/**
	 * @brief      The User Controller Velocity to apply to the Player Entity
	 */
	sf::Vector2f userControllerVelocity;
	/**
	 * @brief      A target level 
	 * 
	 * Set by Teleporters or any other condition that causes a level transition
	 */
	GameLevel *targetLevel = nullptr;
	/**
	 * @brief      The first Gun slot
	 */
	Gun *gun1 = nullptr;
	/**
	 * @brief      The second Gun slot
	 */
	Gun *gun2 = nullptr;
	/**
	 * @brief      The active Gun slot
	 */
	Gun **gun_ptr = &gun1;
	/**
	 * @brief      Overhead Bar showing the ammo level
	 */
	OverheadBar<int>* ammoBar;
	/**
	 * @brief      Current ammo count
	 */
	int ammo = 10;
	/**
	 * @brief      Maximum ammo capacitu
	 */
	int maxAmmo = 15;
	/**
	 * @brief      Number of grenades the player has
	 */
	int grenadeCount = 5;

	/**
	 * @brief      Counter for the Player animations
	 */
	int animation_count;
	/**
	 * @brief      Flag for Player motion animation
	 */
	bool change_motion;

	/**
	 * @brief      Texture for the gun the player is holding
	 */
	sf::Texture *gunTexture;
	/**
	 * @brief      Walking direction, true for right
	 */
	bool dir;
	/**
	 * @brief      Visual X offset values for the Gun+Arm textures
	 */
	float gun_arm_offsetX[2] = { 65.0f, 50.0f};
	/**
	 * @brief      Visual Y offset values for the Gun+Arm textures
	 */
	float gun_arm_offsetY[2] = { 10.0f, -24.0f};
	/**
	 * @brief      Visual X offset values for the Gun+Arm rotation point
	 */
	float gun_rotate_origin_offsetX[2] = { -26.0f ,-28.0f};
	/**
	 * @brief      Visual Y offset values for the Gun+Arm rotation point
	 */
	float gun_rotate_origin_offsetY[2] = { -8.0f ,-10.0f};
	/**
	 * @brief      Visual Angle offset values for the Gun+Arm textures
	 */
	float gun_angle_offset[2] = { 135.0f,45.0f };
	
public:
	
	/**
	 * @brief      Construct a Player Entity at the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Player(GameManager *_manager, GameEntity *parent);
	
	/**
	 * @brief      Construct a Player Entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         The X coordinate relative to the parent node
	 * @param[in]  y         The Y coordinate relative to the parent node
	 */
	Player(GameManager *_manager, GameEntity *parent, float x, float y);
	
	/**
	 * @brief      Construct a Player Entity at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to the parent node
	 */
	Player(GameManager *_manager, GameEntity *parent, sf::Vector2f pos);
	
	/**
	 * @brief      Gets the target level.
	 */
	GameLevel* getTargetLevel();
	
	/**
	 * @brief      Sets the target level.
	 */
	void setTargetLevel(GameLevel *lvl);
	
	/**
	 * @brief      Draw function for the Player Character Entity
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Update function for the Player Character Entity
	 */
	virtual void update() override;
	
	/**
	 * @brief      Gets the player motion.
	 */
	CHARACTER_MOTION getPlayerMotion() const {return current_motion;}

	/**
	 * @brief      Flag for switching Gun, set based on user control
	 */
	bool shouldSwitchGun = false;
	
	/**
	 * @brief      Flag for throwing Grenade, set based on user control
	 */
	bool shouldThrowGrenade = false;
	
	/**
	 * @brief      Teleports the player to the destination level and position
	 *
	 * @param      destLevel  The destination level
	 * @param[in]  pos        The destination position
	 */
	void teleport(GameLevel *destLevel, sf::Vector2f pos);

	
	/**
	 * @brief      Tries to subtract a given amount of ammo from this Character for reloading their gun
	 * 
	 * Adds ammo count checks
	 *
	 * @param[in]  reload  The reload amount
	 *
	 * @return     How much ammo was able to be reloaded
	 */
	virtual int subtractAmmo(int reload) override {
		if (ammo < reload) {
			reload = ammo;
		}
		ammo -= reload;
		return reload;
	}

	/**
	 * @brief      Receives a Gun and stores it in the free slot or swaps the active gun
	 * 
	 * Usually from Collectible
	 *
	 * @param      g     The Gun to receive
	 */
	void receiveGun(Gun *g);
	
	/**
	 * @brief      Receives ammo
	 * 
	 * usually from Collectible
	 *
	 * @param[in]  a     Amount of Ammo
	 */
	void receiveAmmo(int a);
	
	/**
	 * @brief      Receives health restore
	 *
	 * usually from Collectible
	 *
	 * @param[in]  h     Amount of Health to Restore
	 */
	void receiveHealth(int h);
	
	/**
	 * @brief      Gets the amount of Ammo the player has
	 */
	int getAmmo() const {return ammo;}
	
	/**
	 * @brief      Gets the maximum ammo the player can hold
	 */
	int getMaxAmmo() const {return maxAmmo;}
	
	/**
	 * @brief      Gets the current gun.
	 */
	Gun* getCurrentGun() const {return *gun_ptr;}
	
	/**
	 * @brief      Gets the secondary gun.
	 */
	Gun* getSecondaryGun() const {
		if (*gun_ptr == gun1) return gun2;
		else return gun1;
	}
	
	/**
	 * @brief      Gets the grenade count.
	 */
	int getGrenadeCount() const {return grenadeCount;}
};
