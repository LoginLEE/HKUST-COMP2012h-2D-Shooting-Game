#include "ammo_drop.hpp"

#include "utils.hpp"
#include "game_manager.hpp"
#include "player.hpp"

AmmoDrop::AmmoDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, float powerLevel) : Collectible(_manager, parent, pos) {
	ammo = 33 * powerLevel;
	texture = manager->getTexture(AMMO_DROP_TEXTURE, 0);
}

void AmmoDrop::draw(sf::RenderTarget &renderer) const {
	/*
	sf::RectangleShape rect(getSize());
	rect.setFillColor(sf::Color(128, 128, 128));
	rect.setPosition(getTotalPosition());
	rect.setOrigin(rect.getSize() / 2.0f);
	renderer.draw(rect);
	*/

	sf::Sprite sprite(texture);
	sprite.setScale(getSize().x / texture.getSize().x, getSize().y / texture.getSize().y);
	sprite.setPosition(getTotalPosition() - getSize() / 2.0f);
	renderer.draw(sprite);
}

void AmmoDrop::getCollected() {
	manager->getPlayer()->receiveAmmo(ammo);
	setAlive(false);
}