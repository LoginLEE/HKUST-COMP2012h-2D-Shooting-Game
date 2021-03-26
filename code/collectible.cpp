#include "collectible.hpp"

#include "utils.hpp"
#include "game_manager.hpp"
#include "player.hpp"

#include "gun_drop.hpp"
#include "health_drop.hpp"
#include "ammo_drop.hpp"

Collectible::Collectible(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : CollidingEntity(_manager, parent) {
	setPosition(pos);
	setSize(100, 100);
	//setVelocity((utils::randomv2() - sf::Vector2f(0, 0.5)) * 200.0f);
	destabilize(200.0f);
}

void Collectible::update()  {
	/*
	std::vector<CollisionBox> cboxes;
	for (GameEntity *entity : manager->activeLevel->getEntities()) {
		CollidingEntity *ce = dynamic_cast<CollidingEntity*>(entity);
		if (!ce) continue;
		if (!ce->collidesWith(*this)) continue;
		if (Player *player = dynamic_cast<Player*>(ce)) {
			player->collect(this);
			setAlive(false);
		} else if (const Block *block = dynamic_cast<const Block*>(ce)) {
			cboxes.push_back(block->getCollisionBox());
		}
	}*/
	
	processCollectiblePhysics();
	
	if (manager->getPlayer()->collidesWith(*this)) getCollected();
}

void Collectible::processCollectiblePhysics() {
	CollisionBox cbox = getCollisionBox();
	cbox.setSize(cbox.getSize() * 1.2f);
	std::vector<CollidingEntity*> colliding = manager->activeLevel->getCollidingEntities(cbox);
	if (std::count_if(colliding.begin(), colliding.end(), [](CollidingEntity* entity){
		return dynamic_cast<Collectible*>(entity);
	}) > 1/*itself*/) {
		destabilize(200.0f);
	}
	processBouncyPhysics(manager->activeLevel->getBlockCollisionBoxes());
}

void Collectible::draw(sf::RenderTarget &renderer) const {
	sf::RectangleShape rect(getSize());
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(getTotalPosition());
	rect.setOrigin(rect.getSize() / 2.0f);
	renderer.draw(rect);
}

Collectible* Collectible::createCollectibleWithPower(GameManager *manager, GameEntity *parent, sf::Vector2f pos, float powerLevel) {
	float r = utils::randomf();
	if (r < 0.15f) return new GunDrop(manager, parent, pos, powerLevel);
	else if (r < 0.45f) return new HealthDrop(manager, parent, pos, powerLevel);
	else if (r < 0.75f) return new AmmoDrop(manager, parent, pos, powerLevel);
	else return nullptr;
}