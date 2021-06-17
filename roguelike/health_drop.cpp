#include "health_drop.hpp"

#include "utils.hpp"
#include "game_manager.hpp"
#include "player.hpp"

HealthDrop::HealthDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, float powerLevel) : Collectible(_manager, parent, pos) {
	health = powerLevel * 20;
}

void HealthDrop::draw(sf::RenderTarget &renderer) const {
	sf::RectangleShape rect(getSize());
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(getTotalPosition());
	rect.setOrigin(rect.getSize() / 2.0f);
	renderer.draw(rect);
}

void HealthDrop::getCollected() {
	manager->getPlayer()->receiveHealth(health);
	setAlive(false);
}