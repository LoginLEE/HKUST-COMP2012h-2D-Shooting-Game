#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"

/**
 * @brief      Struct to represent different types and amounts of Damage
 * 
 * This serves to allow us to extend the game with different kinds of Damage and Armor
 */
struct Damage {
	float standard = 3.0f;
	float fire = 0.0f;
	float electric = 0.0f;
	float ice = 0.0f;
	
	/**
	 * @brief      Calculate total amount of damage to deal
	 */
	float total() {return standard + fire + electric + ice;}
};