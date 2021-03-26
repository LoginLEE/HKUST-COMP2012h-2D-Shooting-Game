#include "utils.hpp"

namespace utils {
	void panic(std::string msg) {
		cerr << msg << endl;
		cerr << "Exiting..." << endl;
		exit(-1);
	}
	typedef std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31, 
	                             0x9908b0df, 11, 
	                             0xffffffff, 7, 
	                             0x9d2c5680, 15, 
	                             0xefc60000, 18, 1812433253> mt19937;
	extern mt19937 rng;
	long long now() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	float randomf() {
		//return (rng() % 1000000) / 1000000.0f;
		return (rng() / 4294967295.0f);
	}
	float randomf2() {
		//return (rng() % 1000000) / 1000000.0f;
		return randomf()-0.5f;
	}
	sf::Vector2f randomv() {
		return sf::Vector2f(randomf(), randomf());
	}
	sf::Vector2f randomv2() {
		return sf::Vector2f(randomf()-0.5f, randomf()-0.5f);
	}
	sf::Vector2f normalizedRandomv2() {
		return normalize(randomv2());
	}
	float magnitude(const sf::Vector2f &vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	sf::Vector2f normalize(const sf::Vector2f &vec) {
		float mag = magnitude(vec);
		// This is important
		if (mag == 0.0f) mag = 1.0f;
		return vec / mag;
	}
	float distance(sf::Vector2f p1, sf::Vector2f p2) {
		return sqrt( (p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y) );
	}
	float normalizeAngle(float angle) {
		angle = fmod(angle, 360.0f);
		if (angle < -180.0f) return angle + 360.0f;
		else if (angle > 180.0f) return angle - 360.0f;
		else return angle;
	}
	float degreesFromVector(const sf::Vector2f &vec) {
		//cerr << std::atan2(vec.y, vec.x) * 180.0f / M_PI << endl;
		return std::atan2(vec.y, vec.x) * 180.0f / M_PI;
	}
	sf::Vector2f vectorFromDegrees(float angle) {
		angle = std::fmod(angle, 360.0f);
		sf::Vector2f ret = sf::Vector2f(std::cos(angle/180.0f*M_PI), std::sin(angle/180.0f*M_PI));
		//cerr << "angle = " << angle << "  ret = " << ret.x << "," << ret.y << endl;
		return sf::Vector2f(ret);
	}
	float normalizedAngleDiff(float alpha, float beta) {
		return normalizeAngle(alpha - beta);
	}
	float vectorAngleDiff(const sf::Vector2f& alpha, const sf::Vector2f& beta) {
		return normalizedAngleDiff(degreesFromVector(alpha), degreesFromVector(beta));
	}
	sf::Color interpolateColor(sf::Color lowColor, sf::Color highColor, float ratio) {
		int redBegin = lowColor.r, redEnd = highColor.r;
		int greenBegin = lowColor.g, greenEnd = highColor.g;
		int blueBegin = lowColor.b, blueEnd = highColor.b;
		int redDelta = redEnd - redBegin;
		int greenDelta = greenEnd - greenBegin;
		int blueDelta = blueEnd - blueBegin;
		return sf::Color( redBegin + ratio * redDelta, greenBegin + ratio * greenDelta, blueBegin + ratio * blueDelta );
	}
	std::string floatToText(float f) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << f;
		return ss.str();
	}
	std::string textify(float f) {
		return floatToText(f);
	}
	std::string textify(int i) {
		return std::to_string(i);
	}
	std::string textify(long long ll) {
		return std::to_string(ll);
	}
};