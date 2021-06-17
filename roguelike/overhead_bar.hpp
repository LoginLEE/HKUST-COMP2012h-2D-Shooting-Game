#pragma once

#include <SFML/Graphics.hpp>

#include "game_entity.hpp"

/**
 * @brief      Class for Displaying Overhead Bars
 * 
 * This is an entity that displays a dynamically colored bar representing the proportion of some quantity to its maximum possible
 * It takes 2 pointers to fields holding the current and maximum value
 * It is generic over any type that can be cast to a float correctly
 *
 * @tparam     T     Type that can represent a quantity to display in the Overhead Bar
 */
template <typename T>
class OverheadBar : public GameEntity {
private:
	/**
	 * @brief      Pointer to field that stores the current quantity
	 */
	T *current = nullptr;
	/**
	 * @brief      Pointer to field that stores the maximum quantity
	 */
	T *max = nullptr;
	
	/**
	 * @brief      Visual Offset above the Parents head
	 */
	float offset = 30.0f;
	
	/**
	 * @brief      Color of the bar when current is low
	 */
	sf::Color lowColor = sf::Color::Red;
	
	/**
	 * @brief      Color of the bar when current is high
	 */
	sf::Color highColor = sf::Color::Green;

public:
	
	/**
	 * @brief      Create a new Overhead Bar Entity
	 *
	 * @param      manager  The manager
	 * @param      parent   The parent
	 */
	OverheadBar(GameManager* manager, GameEntity *parent) : GameEntity(manager, parent) {
		if (getParentNode() == nullptr) {
			cerr << "Error: OverheadBar cannot be created without a parent" << endl;
			exit(-1);
		}
		setSize(200, 30);
		reposition();
	}

	/**
	 * @brief      Sets the current quantity pointer
	 *
	 * @param      c     { parameter_description }
	 */
	void setCurrent(T *c) {current = c;}
	
	/**
	 * @brief      Sets the maximum quantity pointer
	 *
	 * @param      m     { parameter_description }
	 */
	void setMax(T *m) {max = m;}
	
	/**
	 * @brief      Sets the offset.
	 */
	void setOffset(float o) {offset = o; reposition();}
	
	/**
	 * @brief      Sets the low color.
	 */
	void setLowColor(sf::Color lc) {lowColor = lc;}
	
	/**
	 * @brief      Sets the high color.
	 */
	void setHighColor(sf::Color hc) {highColor = hc;}
	
	/**
	 * @brief      Update the position of the Overhead bar based on any changes to offset
	 */
	void reposition();
	
	/**
	 * @brief      Draw the Overhead Bar
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      Update function
	 */
	virtual void update() override;
};