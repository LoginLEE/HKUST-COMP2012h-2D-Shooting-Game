#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include "global_defines.hpp"
#include <random>
#include <vector>
#include <sstream>
#include <iomanip>

namespace utils {

	/**
	 * @brief      Exit with Error Message
	 *
	 * @param[in]  msg   The error message
	 */
	void panic(std::string msg);

	/**
	 * @brief Mersenne Twister RNG
	 */
	typedef std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31, 
	                             0x9908b0df, 11, 
	                             0xffffffff, 7, 
	                             0x9d2c5680, 15, 
	                             0xefc60000, 18, 1812433253> mt19937;
	extern mt19937 rng;

	/**
	 * @brief      Return the current time
	 */
	long long now();
	
	/**
	 * @brief      Return a random number between 0.0 and 1.0
	 */
	float randomf();
	
	/**
	 * @brief      Return a random number between -0.5 and 0.5
	 */
	float randomf2();
	
	/**
	 * @brief      Return a random vector between <0,0> and <1.0,1.0>
	 */
	sf::Vector2f randomv();
	
	/**
	 * @brief      Return a random vector between <-0.5,-0.5> and <0.5,0.5>
	 */
	sf::Vector2f randomv2();
	
	/**
	 * @brief      Return a random unit vector
	 */
	sf::Vector2f normalizedRandomv2();
	
	/**
	 * @brief      Calculate the magnitude of a vector
	 */
	float magnitude(const sf::Vector2f &vec);
	
	/**
	 * @brief      Normalize a vector to unit magnitude
	 */
	sf::Vector2f normalize(const sf::Vector2f &vec);
	
	/**
	 * @brief      Calculate the distance between 2 points
	 *
	 * @param[in]  p1    Point 1
	 * @param[in]  p2    Point 2
	 */
	float distance(sf::Vector2f p1, sf::Vector2f p2);
	
	#ifndef M_PI
	/**
	 * @brief      Constant PI
	 */
		#define M_PI 3.14159265359f
	#endif
	
	/**
	 * @brief      Get the direction angle of a vector
	 */
	float degreesFromVector(const sf::Vector2f &vec);
	
	/**
	 * @brief      Get a unit vector in the direction of the given angle
	 */
	sf::Vector2f vectorFromDegrees(float angle);
	
	/**
	 * @brief      Get the difference between 2 angles in standard -180 to 180degree range
	 *
	 * @param[in]  alpha  The first angle
	 * @param[in]  beta   The second angle
	 */
	float normalizedAngleDiff(float alpha, float beta);
	
	/**
	 * @brief      Get the angle between 2 vectors in standard -180 to 180degree range
	 *
	 * @param[in]  alpha  The first angle
	 * @param[in]  beta   The second angle
	 */
	float vectorAngleDiff(const sf::Vector2f& alpha, const sf::Vector2f& beta);
	
	/**
	 * @brief      Mixes 2 colors in a given ratio
	 *
	 * @param[in]  lowColor   The low color
	 * @param[in]  highColor  The high color
	 * @param[in]  ratio      The ratio
	 *
	 * @return     newly mixed color
	 */
	sf::Color interpolateColor(sf::Color lowColor, sf::Color highColor, float ratio);
	
	/**
	 * @brief      Group of Drawable Elements
	 */
	struct DrawableGroup {
		std::vector<sf::Transformable*> elements;
		sf::Vector2f anchor = sf::Vector2f(0.0f, 0.0f);
		
		/**
		 * @brief      Add a new Element
		 *
		 * @param      newElem  The new element
		 */
		void add(sf::Transformable *newElem) {
			elements.push_back(newElem);
		}
		/**
		 * @brief      Sets the position.
		 */
		void setPosition(sf::Vector2f newPos) {
			for (sf::Transformable *elem : elements) {
				elem->setPosition( elem->getPosition() - anchor + newPos);
			}
		}
		/**
		 * @brief      Draw the group to the RenderTarget
		 *
		 * @param      renderer  The RenderTarget to render to
		 */
		void draw(sf::RenderTarget &renderer) {
			for (sf::Transformable *elem : elements) {
				sf::Drawable *d = dynamic_cast<sf::Drawable*>(elem);
				if (!d) utils::panic("DrawableGroup has a nondrawable element!!!");
				renderer.draw(*d);
			}
		}
		/**
		 * @brief      Delete all elements in the group
		 */
		void clear() {
			for (sf::Transformable *elem : elements) delete elem;
		}
	};
	
	/**
	 * @brief      Convert a float into a string
	 */
	std::string floatToText(float f);
	
	/**
	 * @brief      Overload of textify for float->string
	 *
	 * @param[in]  f     The float
	 *
	 * @return     The string
	 */
	std::string textify(float f);
	
	/**
	 * @brief      Overload of textify for int->string
	 *
	 * @param[in]  i     The int
	 *
	 * @return     The string
	 */
	std::string textify(int i);
	
	/**
	 * @brief      Overload of textify for longlong->string
	 *
	 * @param[in]  ll     The long long
	 *
	 * @return     The string
	 */
	std::string textify(long long ll);
};