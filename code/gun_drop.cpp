#include "gun_drop.hpp"

#include "utils.hpp"
#include "game_manager.hpp"
#include "player.hpp"

GunDrop::GunDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, Gun *g) : Collectible(_manager, parent, pos) {
	gun = g;
	texture = manager->getTexture(GUN_DROP_TEXTURE, g->getGunType());
}
GunDrop::GunDrop(GameManager *_manager, GameEntity *parent, sf::Vector2f pos, float powerLevel) : Collectible(_manager, parent, pos) {
	if (utils::randomf() < 0.4f + 0.4f * powerLevel / MAX_POWER_LEVEL) {
		gun = Gun::createShotgun(_manager, nullptr, powerLevel);
		texture = manager->getTexture(GUN_DROP_TEXTURE, 1);
	} else {
		gun = Gun::createPistol(_manager, nullptr, powerLevel);
		texture = manager->getTexture(GUN_DROP_TEXTURE, 0);
	}

}

void GunDrop::update() {
	
	processCollectiblePhysics();
	
	if (excitement == 4) {
		if (utils::distance(manager->getPlayer()->getTotalPosition(), getTotalPosition()) < GUN_DROP_COLLECT_DISTANCE)
			getCollected();
	}
	
	excitement--;
}

void GunDrop::draw(sf::RenderTarget &renderer) const {

	sf::Color color = sf::Color(255, 255, 0);
	sf::RectangleShape rect(getSize());
	rect.setFillColor(color);
	rect.setPosition(getTotalPosition());
	rect.setOrigin(rect.getSize() / 2.0f);
	rect.setScale(1.1f, 1.1f);
	if (excitement > 0)renderer.draw(rect);

	sf::Sprite sprite(texture);
	sprite.setScale(getSize().x / texture.getSize().x, getSize().y / texture.getSize().y);
	sprite.setPosition(getTotalPosition() - getSize() / 2.0f);
	
	renderer.draw(sprite);
	
}

void GunDrop::getCollected() {
	manager->getPlayer()->receiveGun(gun);
	setAlive(false);
}