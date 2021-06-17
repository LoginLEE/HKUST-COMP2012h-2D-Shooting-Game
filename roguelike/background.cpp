#include "game_manager.hpp"
#include "background.hpp"

Background::Background(GameManager *_manager, GameEntity *parent = nullptr) : Background(_manager, parent, sf::Vector2f(0, 0)) {}
Background::Background(GameManager *_manager, GameEntity *parent, float x, float y) : Background(_manager, parent, sf::Vector2f(x, y)) {}
Background::Background(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : GameEntity(_manager, parent, 0, 0) {
	setPosition(pos);
	setSize(sf::Vector2f(BACKGROUND_SIZE, BACKGROUND_SIZE));
	setDrawPriority(-5);
	setUpdatePriority(-5);
	texture = manager->getTexture(BACKGROUND_TEXTURE, 0);
}

void Background::draw(sf::RenderTarget &renderer) const {
	
	sf::Sprite sprite(texture);

	sprite.setScale(getSize().x / texture.getSize().x, getSize().y / texture.getSize().y);
	sprite.setPosition(getTotalPosition().x - BACKGROUND_SIZE, getTotalPosition().y - BACKGROUND_SIZE);
	sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f));
	renderer.draw(sprite);
	
}