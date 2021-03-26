#include "grenade.hpp"
#include "game_manager.hpp"
#include "game_level.hpp"
#include "bullet.hpp"

Grenade::Grenade(GameManager *_manager, GameEntity *parent) : CollidingEntity(_manager, parent) {
	setSize(50, 50);
	setDrawPriority(5);
}

void Grenade::update() {
	
	//processBasicPhysics(manager->activeLevel->getBlockCollisionBoxes());
	processBouncyPhysics(manager->activeLevel->getBlockCollisionBoxes());
	
	if (waitTime-- > 0) return;
	
	for (int i = 0; i < num; i++) {
		Bullet *b = new Bullet(manager, manager->activeLevel);	
		b->setPosition(getTotalPosition());
		sf::Vector2f moveDirection = utils::normalize(utils::randomv2());
		b->setVelocity(moveDirection * spd);
		b->setDamage(dmg);
		b->setTimeToLive(ttl);
		// manager->activeLevel->addChild(b);
	}
	
	setAlive(false);
}

void Grenade::draw(sf::RenderTarget &renderer) const {
	sf::CircleShape shape(50);
	shape.setFillColor(sf::Color::Blue);
	shape.setPosition(getTotalPosition());
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	renderer.draw(shape);
}