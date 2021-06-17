#include "character.hpp"

#include "game_level.hpp"
#include "block.hpp"
#include "game_manager.hpp"

Character::Character(GameManager *_manager, GameEntity *parent = nullptr) : Character(_manager, parent, sf::Vector2f(0, 0)) {}
Character::Character(GameManager *_manager, GameEntity *parent, float x, float y) : Character(_manager, parent, sf::Vector2f(x, y)) {}
Character::Character(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : CollidingEntity(_manager, parent) {
	setPosition(pos);
}

void Character::takeDamage(Damage damage) {
	health -= damage.total();
}