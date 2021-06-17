#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "game_entity.hpp"
#include "damage.hpp"

class GameLevel;
class Player;
class GameManager;
class CollisionRay;

/**
 * @brief      Enum for different Gun Textures
 */
enum GUN_TEXTURE
{
	PISTOL_LEFT = 0,
	PISTOL_RIGHT,
	SHOTGUN_LEFT,
	SHOTGUN_RIGHT	
};

/**
 * @brief      Class for Gun Entities
 */
class Gun : public GameEntity {
private:
	/**
	 * @brief      True for right, False for left
	 */
	bool pointing_direction;
	float powerLevel = -999.0f;
	
protected:
	/**
	 * @brief      Amount and Type of Damage
	 */
	Damage dmg;
	/**
	 * @brief       Rate of Fire
	 */
	float rof = 15.0f; 
	/**
	 * @brief       Bullets per Shot
	 */
	int bps = 1; 
	/**
	 * @brief       Capacity (of each reload)
	 */
	int cap = 15; 
	/**
	 * @brief       Reload time
	 */
	float rld = 1.0f; 
	/**
	 * @brief       Bullet speed
	 */
	float spd = 150.0f; 
	/**
	 * @brief       Spread angle in degrees
	 */
	float spr = 30.0f; 
	/**
	 * @brief       Time-to-live of bullets (in updates)
	 */
	int ttl = 10; 
	/**
	 * @brief      Scaling Factors for the Gun texture
	 */
	float gun_scale[2] = { 1.0f, 0.8f };
	/**
	 * @brief      Arm Image Offset X
	 */
	float arm_offsetX[4] = { -55.0f, 50.0f ,-70.0f, 60.0f };
	/**
	 * @brief      Arm Image Offset Y
	 */
	float arm_offsetY[4] = { -10.0f, -24.0f,25.0f, 20.0f };
	/**
	 * @brief      Arm Rotate Origin Offset X
	 */
	float rotate_origin_offsetX[4] = { -26.0f ,-28.0f, -55.0f, -60.0f};
	/**
	 * @brief      Arm Rotate Origin Offset Y
	 */
	float rotate_origin_offsetY[4] = { -8.0f ,-10.0f,-15.0f ,-5.0f };

	/**
	 * @brief      Type of Gun
	 */
	enum GUN_TYPE
	{
		PISTOL = 0,
		SHOTGUN
	};

	/**
	 * @brief      Current Gun Texture
	 */
	GUN_TEXTURE gun_texture = PISTOL_LEFT;

	/**
	 * @brief      Current Gun Type
	 */
	GUN_TYPE gun_type = PISTOL;
	
	/**
	 * @brief      Is this gun currently shooting?
	 */
	bool shooting = false;
	/**
	 * @brief      In milliseconds, when did we last fire?
	 */
	long long lastShootTime = 0; // in milliseconds
	/**
	 * @brief      In milliseconds, when did we start shooting?
	 */
	long long shootingStartTime = 0;
	/**
	 * @brief      Number of bullets loaded into the gun currently
	 */
	int cap_counter = 0;
	/**
	 * @brief      Number of bullets that were loaded into the gun when it started shooting
	 */
	int shootingStartCap = 0;
	/**
	 * @brief      Is currently reloading?
	 */
	bool reloading = true;
	/**
	 * @brief      In milliseconds, when did we start reloading
	 */
	long long reloadingStartTime = -1000000000; // in milliseconds
	/**
	 * @brief      Pointer to Gun Textures
	 */
	sf::Texture *texture;
	/**
	 * @brief      Pointing Direction of the Gun
	 */
	sf::Vector2f direction = sf::Vector2f(1.0f, 0.0f);
	/**
	 * @brief      Fire one bullet
	 */
	void fire();

	/**
	 * @brief      Construct a new Gun
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	Gun(GameManager *_manager, GameEntity *parent);
		
public:
	
	/**
	 * @brief      Update function for this Gun
	 */
	virtual void update();
	
	/**
	 * @brief      Draw function for this Gun
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const;
	
	/**
	 * @brief      Gets the direction this gun is aimed
	 */
	sf::Vector2f getDirection() {return direction;}
	
	/**
	 * @brief      Sets the direction this gun is pointed
	 */
	void setDirection(sf::Vector2f dir);
	
	/**
	 * @brief      Rotate the gun by a given angle
	 *
	 * @param[in]  angle  The angle
	 */
	void rotateBy(float angle);
	
	/**
	 * @brief      Starts shooting.
	 */
	void startShooting();
	
	/**
	 * @brief      Stops shooting.
	 */
	void stopShooting();
	
	/**
	 * @brief      Starts reloading
	 */
	void reload();
	
	/**
	 * @brief      Is the gun shooting right now?
	 */
	bool get_shooting(){ return shooting; }
	
	/**
	 * @brief      Gets the left/right direction of this gun
	 */
	bool getDir() { return pointing_direction; }
	
	/**
	 * @brief      Returns the gun type
	 *
	 * @return     The gun type.
	 */
	int getGunType(){ return gun_type; }

	/**
	 * @brief      Gets the last shooting time.
	 *
	 * @return     The last shooting time.
	 */
	long long getLastShootingTime() { return lastShootTime; }

	/**
	 * @brief      Gets the maximum capacity.
	 *
	 * @return     The maximum capacity.
	 */
	int getMaxCapacity() const {return cap;}
	
	/**
	 * @brief      Gets the ammo load amount
	 *
	 * @return     The capacity.
	 */
	float getCapacity() { return cap_counter; }
	
	/**
	 * @brief      Get the point of origin of bullets from this Gun
	 *
	 * @return     { description_of_the_return_value }
	 */
	sf::Vector2f bulletOrigin() const;
	
	/**
	 * @brief      Gets the shooting collision ray for aim checking.
	 *
	 * @return     The shooting collision ray.
	 */
	CollisionRay getShootingRay() const;

	/**
	 * @brief      Creates a gun based on GUN_TYPE and powerLevel
	 *
	 * @param[in]  ty          Type of Gun
	 * @param      _manager    The global GameManager pointer
	 * @param      parent      The parent GameEntity node
	 * @param[in]  powerLevel  The power level
	 *
	 * @return     A new Gun
	 */
	static Gun* createGun(GUN_TYPE ty, GameManager *_manager, GameEntity *parent, float powerLevel) {
		switch (ty) {
			case PISTOL:
				return createPistol(_manager, parent, powerLevel);
				break;
			case SHOTGUN:
				return createShotgun(_manager, parent, powerLevel);
				break;
		}
	}
	
	/**
	 * @brief      Creates a pistol.
	 *
	 * @param      _manager    The global GameManager pointer
	 * @param      parent      The parent GameEntity node
	 * @param[in]  powerLevel  The power level
	 *
	 * @return     A new Pistol
	 */
	static Gun* createPistol(GameManager *_manager, GameEntity *parent, float powerLevel) {
		Gun *gun = new Gun(_manager, parent);
		gun->powerLevel = powerLevel;
		gun->rof = 3.0f + 3 * powerLevel / MAX_POWER_LEVEL;
		gun->bps = 1;
		gun->cap = 5 + 10 * powerLevel / MAX_POWER_LEVEL;
		gun->rld = 2.0f - 1.5f * powerLevel / MAX_POWER_LEVEL;
		gun->spd = 100.0f + 100.0f * powerLevel / MAX_POWER_LEVEL;
		gun->spr = 35.0f - 30.0f * powerLevel / MAX_POWER_LEVEL;
		gun->pointing_direction = 0;
		gun->gun_type = PISTOL;
		return gun;
	}
	
	/**
	 * @brief      Creates a shotgun.
	 *
	 * @param      _manager    The global GameManager pointer
	 * @param      parent      The parent GameEntity node
	 * @param[in]  powerLevel  The power level
	 *
	 * @return     A new Shotgun
	 */
	static Gun* createShotgun(GameManager *_manager, GameEntity *parent, float powerLevel) {
		Gun *gun = new Gun(_manager, parent);
		gun->powerLevel = powerLevel;
		gun->rof = 1.0f + 2.0f * powerLevel / MAX_POWER_LEVEL;
		gun->bps = std::round(2 + 3 * powerLevel / MAX_POWER_LEVEL);
		gun->cap = 2 + 6 * powerLevel / MAX_POWER_LEVEL;
		gun->rld = 3.0f - 2.0f * powerLevel / MAX_POWER_LEVEL;
		gun->spd = 100.0f + 100.0f * powerLevel / MAX_POWER_LEVEL;
		gun->spr = 90.0f - 30.0f * powerLevel / MAX_POWER_LEVEL;
		gun->pointing_direction = 0;
		gun->gun_type = SHOTGUN;
		return gun;
	}
	
	
	/**
	 * @brief      Gets the damage amount.
	 *
	 * @return     The damage amount.
	 */
	float getDamage() const {return dmg.standard;}
	
	/**
	 * @brief      Gets the shooting speed.
	 *
	 * @return     The shooting speed.
	 */
	float getSpeed() const {return spd;}
	
	/**
	 * @brief      Gets the rate of fire.
	 *
	 * @return     The rate of fire.
	 */
	float getRof() const {return rof;}
	
	/**
	 * @brief      Gets the reload capacity.
	 *
	 * @return     The reload capacity.
	 */
	int getCap() const {return cap;}
	
	/**
	 * @brief      Gets the reload time.
	 *
	 * @return     The reload time.
	 */
	float getReload() const {return rld;}
	
	/**
	 * @brief      Gets the accuracy.
	 *
	 * @return     The accuracy.
	 */
	float getAccuracy() const {return 1.0f - (spr / 180.0f);}
	
	/**
	 * @brief      Gets the range.
	 *
	 * @return     The range.
	 */
	float getRange() const {return spd * ttl;}
	
	/**
	 * @brief      Gets the power level.
	 *
	 * @return     The power level.
	 */
	float getPowerLevel() const {return powerLevel;}
	
	/**
	 * @brief      Determines if reloading.
	 *
	 * @return     True if reloading, False otherwise.
	 */
	bool isReloading() const {return reloading;}
	
};

