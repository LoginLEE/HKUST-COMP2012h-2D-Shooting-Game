#pragma once

#include "global_defines.hpp"
#include <algorithm>
#include <SFML/Graphics.hpp>

/**
 * @brief      Class for representing and calculating with Collision Boxes
 */
class CollisionBox {
private:
	/**
	 * @brief Upper Lefthand Corner Point of this Collision Box
	 */
	sf::Vector2f upLeft = sf::Vector2f(0.0f, 0.0f);
	
	/**
	 * @brief Lower Righthand Corner Point of this Collision Box
	 */
	sf::Vector2f downRight = sf::Vector2f(0.0f, 0.0f);
	
	/**
	 * Is this a valid Collision Box, used to help collision checing
	 */
	bool valid = true;
	
public:
	/**
	 * @brief      Constructs a default Collision Box
	 */
	CollisionBox();
	
	/**
	 * @brief      Constructs a Collision Box of given Center Point and Size
	 *
	 * @param[in]  center  The Center Point
	 * @param[in]  size    The Size of the box
	 */
	CollisionBox(sf::Vector2f center, sf::Vector2f size);
	
	/**
	 * @brief      Sets this Collision Box to represent the given Center Point and Size
	 *
	 * @param[in]  center  The Center Point
	 * @param[in]  size    The Size of the box
	 */
	void set(sf::Vector2f center, sf::Vector2f size);
	
	/**
	 * @brief      Sets the Center Point
	 *
	 * @param[in]  center  The center point
	 */
	void setCenter(sf::Vector2f center);
	
	/**
	 * @brief      Sets the size of this Collision Box.
	 *
	 * @param[in]  size  The size
	 */
	void setSize(sf::Vector2f size);
	
	/**
	 * @brief      Gets Upper Lefthand Corner Point of this Collision Box
	 */
	sf::Vector2f getUpLeft() const {return upLeft;};
	
	/**
	 * @brief      Gets Lower Righthand Corner Point of this Collision Box
	 */
	sf::Vector2f getDownRight() const {return downRight;};
	

	/**
	 * @brief      Gets Upper Righthand Corner Point of this Collision Box
	 */
	sf::Vector2f getUpRight() const {return sf::Vector2f(downRight.x, upLeft.y);};
	

	/**
	 * @brief      Gets Lower Lefthand Corner Point of this Collision Box
	 */
	sf::Vector2f getDownLeft() const {return sf::Vector2f(upLeft.x, downRight.y);};

	/**
	 * @brief      Gets the Center Point of this Collision Box
	 *
	 * @return     The center point.
	 */
	sf::Vector2f getCenter() const;
	
	/**
	 * @brief      Gets the size of this Collision Box.
	 *
	 * @return     The size.
	 */
	sf::Vector2f getSize() const;
	
	/**
	 * @brief      Determines if valid.
	 *
	 * @return     True if valid, False otherwise.
	 */
	bool isValid() const;

	/**
	 * @brief      Determines if it contains the given point.
	 *
	 * @param[in]  point  The point to check
	 *
	 * @return     True if contains point, False otherwise.
	 */
	bool containsPoint(sf::Vector2f point) const {
		return (point.x >= upLeft.x) && (point.x <= downRight.x) && (point.y >= upLeft.y) && (point.y <= downRight.y);
	}
	
	/**
	 * @brief      Intersects 2 Collision Boxes to give a new Collision Box representing the overlapping are
	 *
	 * @param[in]  other  The other Collision Box
	 *
	 * @return     The Collision Box representing overlapping area
	 */
	CollisionBox intersect(const CollisionBox &other) const;
	
	/**
	 * @brief      Checks if 2 Collision Boxes are intersecting each other
	 * 
	 * It does this by calculating the intersection of the 2 collision boxes and checking if the resulting collision box is valid
	 *
	 * @param[in]  other  The other Collision Box
	 *
	 * @return     true if the 2 Collision Boxes are intersection/overlapping
	 */
	bool checkIntersection(const CollisionBox &other) const;
	
	/**
	 * @brief      Intersects 2 Collision Boxes to give a new Collision Box representing the overlapping are
	 *
	 * @param[in]  a     A Collision Box
	 * @param[in]  b     B Collision Box
	 *
	 * @return     The Collision Box of the overlapping region
	 */
	static CollisionBox intersect(const CollisionBox &a, const CollisionBox &b);
	
	/**
	 * @brief      Checks if 2 Collision Boxes are intersecting each other
	 * 
	 * It does this by calculating the intersection of the 2 collision boxes and checking if the resulting collision box is valid
	 *
	 * @param[in]  a     A Collision Box
	 * @param[in]  b     B Collision Box
	 *
	 * @return     true if the 2 Collision Boxes are intersection/overlapping
	 */
	static bool checkIntersection(const CollisionBox &a, const CollisionBox &b);

	/**
	 * @brief      Draws the Collision Box with a chosen Color as a solid coloured Rectangle, for Debugging
	 *
	 * @param      renderer  The RenderTarget to draw to
	 * @param[in]  color     The Color of Rectangle to Draw (default: Yellow)
	 */
	void draw(sf::RenderTarget &renderer, sf::Color color = sf::Color::Yellow) const {
		sf::RectangleShape rect(getSize());
		rect.setFillColor(color);
		rect.setPosition(upLeft);
		renderer.draw(rect);
	}
};