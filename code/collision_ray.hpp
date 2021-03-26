#pragma once

#include "global_defines.hpp"
#include <algorithm>
#include <SFML/Graphics.hpp>
// #include "utils.hpp"

class CollisionBox;

/**
 * @brief      Class for representing and calculating with Collision Rays
 * 
 * Collision Rays can be thought of as a line with 1 end point defined (the origin point)
 * extending infinitely in the direction of a certain vector (the direction vector)
 */
class CollisionRay {
private:

	/**
	 * @brief The Origin Point of this Ray
	 */
	sf::Vector2f origin = sf::Vector2f(0.0f, 0.0f);
	/**
	 * @brief The Direction vector of this Ray
	 */
	sf::Vector2f direction = sf::Vector2f(0.0f, 0.0f);

public:

	/**
	 * @brief      Constructs a Collision Ray with the given Origin Point and Direction Vector
	 *
	 * @param[in]  origin     The Origin Point
	 * @param[in]  direction  The Direction Vector
	 */
	CollisionRay(sf::Vector2f origin, sf::Vector2f direction) : origin(origin), direction(direction) {}

	/**
	 * @brief      Constructs a Collision Box representing a point "at infinity" (very far away) on this Collision Ray
	 */
	CollisionBox atInfinity() const {
		return CollisionBox(origin + direction*1000000000.0f, sf::Vector2f(1.0, 1.0));
	}

	/**
	 * @brief      Constructs a Collision Ray from 2 Points
	 *
	 * @param[in]  origin  The Origin Point
	 * @param[in]  dest    The Destination Point for the Ray to Point at
	 *
	 * @return     Collision ray from the Origin Point pointing at the Destination Point
	 */
	static CollisionRay fromTwoPoints(sf::Vector2f origin, sf::Vector2f dest) {
		return CollisionRay(origin, dest-origin);
	}
	/**
	 * @brief      Constructs a new Collision Ray from this Origin Point pointing at the given Destination point
	 *
	 * @param[in]  dest  The destination point to point at
	 *
	 * @return     Collision ray from this Origin Point pointing at the Destination Point
	 */
	CollisionRay toOtherPoint(sf::Vector2f dest) {
		return CollisionRay(origin, dest-origin);
	}

	/**
	 * @brief      Gets the Origin Point of this Ray
	 */
	sf::Vector2f getOrigin() { return origin; };
	
	/**
	 * @brief      Gets the angle of the direction vector
	 */
	float getAngle() { return utils::degreesFromVector(direction); };

	/**
	 * @brief      Checks if this Collision Ray intersects/passes through the given Collision Box
	 *
	 * @param[in]  cbox  The Collision Box to check
	 *
	 * @return     true if this Collision Ray intersects the given Collision Box
	 */
	bool checkIntersection(const CollisionBox& cbox) {
		CollisionRay ray1 = toOtherPoint(cbox.getUpRight());
		CollisionRay ray2 = toOtherPoint(cbox.getDownLeft());
		if (betweenTwoRays(ray1, ray2)) return true;
		CollisionRay ray3 = toOtherPoint(cbox.getUpLeft());
		if (betweenTwoRays(ray1, ray3)) return true;
		if (betweenTwoRays(ray2, ray3)) return true;
		CollisionRay ray4 = toOtherPoint(cbox.getDownRight());
		if (betweenTwoRays(ray1, ray4)) return true;
		if (betweenTwoRays(ray2, ray4)) return true;
		if (betweenTwoRays(ray3, ray4)) return true;
		return false;
	}

	/**
	 * @brief      Find the closest intersecting Collision Box from the given vector
	 *
	 * @param[in]  cboxes   Vector of Collision Boxes to check against
	 * @param      closest  Reference to 1. the initial closest Collision Box, 2. location to return the new closest intersection Collision Box
	 *
	 * @return     The closest intersection Collision Box
	 */
	bool getFirstIntersection(const std::vector<CollisionBox>& cboxes, CollisionBox& closest) {
		//CollisionBox closest = CollisionBox(sf::Vector2f(1000000, 1000000), sf::Vector2f(1.0, 1.0));
		float closest_dist = utils::magnitude(closest.getCenter() - origin);
		bool foundAnyCloser = false;

		for (CollisionBox cbox : cboxes) {
			float dist = utils::magnitude(cbox.getCenter() - origin);
			
			if (dist > closest_dist) continue;
			if (!checkIntersection(cbox)) continue;
			
			closest = cbox;
			closest_dist = dist; 
			foundAnyCloser = true;
		}

		return foundAnyCloser;
	}

	/**
	 * @brief      Checks if this Collision Ray lies 'inbetween' the 2 Given Collision Rays
	 *
	 * @param[in]  cray1  Collision Ray 1
	 * @param[in]  cray2  Collision Ray 2
	 *
	 * @return     true if the collision ray lies between the 2 other rays
	 */
	bool betweenTwoRays(const CollisionRay& cray1, const CollisionRay& cray2) {
		float angleDiff1 = utils::vectorAngleDiff(direction, cray1.direction);
		if (angleDiff1 > 135.0f || angleDiff1 < -135.0f) return false;
		float angleDiff2 = utils::vectorAngleDiff(direction, cray2.direction);
		if (angleDiff2 > 135.0f || angleDiff2 < -135.0f) return false;
		if (angleDiff1 < 0 && angleDiff2 < 0) return false;
		if (angleDiff1 > 0 && angleDiff2 > 0) return false;
		// cerr << angleDiff1 << ", " << angleDiff2 << endl;
		return true;
	}

	/**
	 * @brief      Draws the Collision Ray as a line of given color, for debugging Purposes
	 *
	 * @param      renderer  The RenderTarget to draw to
	 * @param[in]  color     The Color of Line to Draw (default: Yellow)
	 */
	void draw(sf::RenderTarget &renderer, sf::Color color = sf::Color::Yellow) const {
		sf::RectangleShape rect(sf::Vector2f(100000, 10));
		rect.setFillColor(color);
		rect.setPosition(origin);
		// rect.setOrigin(0, rect.getSize().y / 2.0f);
		rect.setRotation(utils::degreesFromVector(direction));
		renderer.draw(rect);
	}
	
};