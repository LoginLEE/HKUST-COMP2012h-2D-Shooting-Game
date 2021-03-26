#include "teleporter.hpp"
#include "player.hpp"
#include "game_level.hpp"

Teleporter::Teleporter(GameManager *_manager, GameEntity *parent = nullptr) : Teleporter(_manager, parent, sf::Vector2f(0, 0)) {}
Teleporter::Teleporter(GameManager *_manager, GameEntity *parent, float x, float y) : Teleporter(_manager, parent, sf::Vector2f(x, y)) {}
Teleporter::Teleporter(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : Block(_manager, parent) {
	setPosition(pos);
	setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
	setCollisionBox(CollisionBox(getTotalPosition(), getSize()));
	setUpdatePriority(1000);
	texture.loadFromFile(TEXTURE_PATH + "block/teleporter.png");
}

void Teleporter::update() {
	
	if (CURRENT_TICK < inactiveUntil) return;
	
	if (!isActive()) return;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
		CollisionBox cbox = getCollisionBox();
		cbox.setSize(sf::Vector2f(cbox.getSize().x, cbox.getSize().y * 1.1f));
		cbox.setCenter(cbox.getCenter() + sf::Vector2f(0, cbox.getSize().y * -0.05f));
		std::vector<CollidingEntity*> colliding = level->getCollidingEntities(cbox);
		std::vector<CollidingEntity*>::iterator it = std::find_if(colliding.begin(), colliding.end(),
		[](CollidingEntity *e){return dynamic_cast<Player*>(e);});
		if (it != colliding.end()) {
			Player *p = dynamic_cast<Player*>(*it);
			p->teleport(associate->getLevel(), associate->getPosition() + sf::Vector2f(0, -TELEPORTER_SPAWN_OFFSET - p->getSize().y));
			overheat();
			associate->overheat();
		}
	}
}

void Teleporter::draw(sf::RenderTarget &renderer) const {
	/*
	sf::RectangleShape rect(getSize());
	rect.setFillColor(sf::Color::Black);
	rect.setPosition(getTotalPosition());
	rect.setOrigin(rect.getSize() / 2.0f);
	renderer.draw(rect);
	*/
	sf::Sprite sprite(texture);

	sprite.setScale(getSize().x / texture.getSize().x, getSize().y / texture.getSize().y * 3.0f);
	sprite.setPosition(getTotalPosition().x, getTotalPosition().y - getSize().y);
	sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f));
	renderer.draw(sprite);
}