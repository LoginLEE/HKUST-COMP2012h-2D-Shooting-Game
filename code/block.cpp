#include "block.hpp"

Block::Block(GameManager *_manager, GameEntity *parent = nullptr) : Block(_manager, parent, sf::Vector2f(0, 0)) {}
Block::Block(GameManager *_manager, GameEntity *parent, float x, float y) : Block(_manager, parent, sf::Vector2f(x, y)) {}
Block::Block(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : CollidingEntity(_manager, parent) {
	setPosition(pos);
	setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
	setCollisionBox(CollisionBox(getTotalPosition(), getSize()));
}