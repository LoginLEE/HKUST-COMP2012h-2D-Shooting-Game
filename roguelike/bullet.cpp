#include "bullet.hpp"

#include "game_level.hpp"
#include "block.hpp"
#include "character.hpp"
#include "game_manager.hpp"

Bullet::Bullet(GameManager *_manager, GameEntity *parent = nullptr) : Bullet(_manager, parent, sf::Vector2f(0, 0)) {}
Bullet::Bullet(GameManager *_manager, GameEntity *parent, float x, float y) : Bullet(_manager, parent, sf::Vector2f(x, y)) {}
Bullet::Bullet(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : CollidingEntity(_manager, parent) {
	setPosition(pos);
	setSize(60, 60);
}

Damage Bullet::getDamage() const {return damage;}
void Bullet::setDamage(Damage dmg) {damage = dmg;}
void Bullet::setTimeToLive(int ttl) {timeToLive = ttl;}

void Bullet::draw(sf::RenderTarget &renderer) const {
	sf::CircleShape shape(30);
	shape.setFillColor(sf::Color::Black);
	shape.setPosition(getTotalPosition());
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	renderer.draw(shape);
}

void Bullet::update() {
	
	if (--timeToLive < 0) {
		setAlive(false);
		return;
	}
	// Lol
	setPosition(getPosition() + getVelocity());

	for (GameEntity *entity : manager->activeLevel->getEntities()) {
		CollidingEntity *ce = dynamic_cast<CollidingEntity*>(entity);
		if (!ce) continue;
		if (!ce->collidesWith(*this)) continue;
		if (Character *character = dynamic_cast<Character*>(ce)) {
			character->takeDamage(damage);
			setAlive(false);
		} else if (Block *block = dynamic_cast<Block*>(ce)) {
			setAlive(false);
		}
	}
	
	// Lol
	// setPosition(getPosition() + getVelocity());
}