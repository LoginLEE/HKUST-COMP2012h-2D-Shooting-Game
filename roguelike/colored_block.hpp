#pragma once

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "colliding_entity.hpp"
#include "block.hpp"

/**
 * @brief      Class for Simple Solid Colored Blocks
 */
class ColoredBlock : public Block {
private:
	/**
	 * @brief the Color of this Block
	 */
	sf::Color blockColor = sf::Color::Blue;
public:
	
	/**
	 * @brief      Construct a Colored Block with given Color at the default position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      color     The desired Color of the Block
	 * @param      parent    The parent GameEntity node
	 */
	ColoredBlock(GameManager *_manager, sf::Color color, GameEntity *parent) 
		: ColoredBlock(_manager, color, parent, 0.0f, 0.0f) {};
	
	/**
	 * @brief      Construct a Colored Block with given Color at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      color     The desired Color of the Block
	 * @param      parent    The parent GameEntity node
	 * @param[in]  x         x coordinate
	 * @param[in]  y         y coordinate
	 */
	ColoredBlock(GameManager *_manager, sf::Color color, GameEntity *parent, float x, float y)
		: ColoredBlock(_manager, color, parent, sf::Vector2f(x, y)) {};
	
	/**
	 * @brief      Construct a Colored Block with given Color at the given position
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      color     The desired Color of the Block
	 * @param      parent    The parent GameEntity node
	 * @param[in]  pos       The position relative to its parent
	 */
	ColoredBlock(GameManager *_manager, sf::Color color, GameEntity *parent, sf::Vector2f pos)
		: Block(_manager, parent, pos), blockColor(color) {};

	/**
	 * @brief      Sets the color.
	 *
	 * @param[in]  color  The color
	 */
	void setColor(sf::Color color) {blockColor = color;};
	
	/**
	 * @brief      Gets the color.
	 *
	 * @return     The color.
	 */
	sf::Color getColor() {return blockColor;};

	/**
	 * @brief      Static helper function to get a color based on an index, used for map file loading
	 *
	 * @param[in]  index  The index
	 *
	 * @return     Color corresponding to given index
	 */
	static const sf::Color blockColorMap(int index) {
		static const sf::Color map[6] = {
			sf::Color::Blue, sf::Color::Red, sf::Color::Yellow, sf::Color::Green, sf::Color::Black, sf::Color::White	
		};
		return map[index];
	}

	/**
	 * @brief      Draw this Colored Block to the RenderTarget
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override {
		sf::RectangleShape rect(getSize());
		rect.setFillColor(blockColor);
		rect.setPosition(getTotalPosition());
		rect.setOrigin(rect.getSize() / 2.0f);
		renderer.draw(rect);
	}
};