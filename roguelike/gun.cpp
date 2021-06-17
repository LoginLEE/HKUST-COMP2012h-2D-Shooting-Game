#include "gun.hpp"
#include "utils.hpp"
#include "bullet.hpp"
#include "game_level.hpp"
#include "game_manager.hpp"
#include "character.hpp"
#include "collision_ray.hpp"

#define USE_GUN_TEXTURE

Gun::Gun(GameManager *_manager, GameEntity *parent) : GameEntity(_manager, parent, 0, 5) {
#ifndef USE_GUN_TEXTURE
	setSize(500, 50);
#else
	setSize(150, 100);

	//load texture from resource manager
	texture = new sf::Texture[4];
	for (int i = 0; i < 4; i++)texture[i] = manager->getTexture(GUN_TEXTURE, i);

#endif
}

sf::Vector2f Gun::bulletOrigin() const {
	return getTotalPosition() + direction * (getSize().x + 20.0f);
}

void Gun::fire() {
	//cerr << "fire" << endl;
	Bullet *b = new Bullet(manager, manager->activeLevel);	
	b->setPosition(bulletOrigin());
	float baseAngle = utils::degreesFromVector(direction);
	float moveAngle = baseAngle + (utils::randomf() * spr - spr/2.0f);
	sf::Vector2f moveDirection = utils::normalize(utils::vectorFromDegrees(moveAngle));
	b->setVelocity(moveDirection * spd);
	b->setDamage(dmg);
	b->setTimeToLive(ttl);
	// b->setCurrentLevel(currentLevel);
	// manager->activeLevel->addChild(b);
}

void Gun::update() {
	pointing_direction = !(utils::degreesFromVector(direction) < -90 || utils::degreesFromVector(direction) > 90);

	if (reloading) {
		if (utils::now() - reloadingStartTime >= rld * 1000) {
			reloading = false;
			if (Character* character = dynamic_cast<Character*>(getParentNode())) {
				cap_counter += character->subtractAmmo(cap-cap_counter);
				// Lol xd
			}
		}
		return;
	}
	
	if (cap_counter == 0) {
		reload();
		return;
	}

	if (shooting) {
		if (utils::now() - shootingStartTime >= (shootingStartCap - cap_counter) * 1000 * 1.0f/rof) {
			for (int i = 0; i < bps; i++) fire();
			cap_counter--;
			lastShootTime = utils::now();
			return;
		}
	}
}

void Gun::draw(sf::RenderTarget &renderer) const {
#ifndef USE_GUN_TEXTURE
	sf::RectangleShape rect(getSize());
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(getTotalPosition());
	rect.setOrigin(0, rect.getSize().y / 2.0f);
	rect.setRotation(utils::degreesFromVector(direction));
	renderer.draw(rect);
#else
	sf::Sprite sprite(texture[pointing_direction + gun_type*2]);
	sprite.setPosition(getTotalPosition().x + arm_offsetX[pointing_direction + gun_type * 2], getTotalPosition().y + arm_offsetY[pointing_direction + gun_type * 2]);
	sprite.setScale(getSize().x / texture[pointing_direction].getSize().x * gun_scale[gun_type], getSize().y / texture[pointing_direction].getSize().y * gun_scale[gun_type]);
	sprite.setOrigin(texture[pointing_direction + gun_type * 2].getSize().x + rotate_origin_offsetX[pointing_direction + gun_type * 2], texture[pointing_direction + gun_type * 2].getSize().y + rotate_origin_offsetY[pointing_direction + gun_type * 2]);
	sprite.setRotation(utils::degreesFromVector(direction));
	
	if (const Player* player = dynamic_cast<const Player*>(getParentNode()))
	{
		//cerr << "now motion" << player->getPlayerMotion() << endl;
		if (player->getPlayerMotion() < 2 || player->getPlayerMotion() > 17) renderer.draw(sprite);
	}

	if (const Enemy* enemy = dynamic_cast<const Enemy*>(getParentNode()))
	{
		//cerr << "now motion" << player->getPlayerMotion() << endl;
		if (enemy);renderer.draw(sprite);
	}
#endif
	 if (DISPLAY_DEBUGGING_STUFF) {
		 getShootingRay().draw(renderer);
	}
}

CollisionRay Gun::getShootingRay() const {
	return CollisionRay(bulletOrigin(), this->direction);
}

void Gun::setDirection(sf::Vector2f pointing_direction) {
	direction = utils::normalize(pointing_direction);
}
void Gun::rotateBy(float angle) {
	float deg = utils::degreesFromVector(direction) + angle;
	direction = utils::vectorFromDegrees(deg);
}

void Gun::startShooting() {
	if (reloading) return;
	if (shooting) return;
	if (utils::now() - lastShootTime < 1000 * 1.0f/rof) return;
	shootingStartTime = utils::now();
	shootingStartCap = cap_counter;
	shooting = true;
}

void Gun::stopShooting() {
	if (reloading) return;
	if (!shooting) return;
	shooting = false;
}

void Gun::reload() {
	if (reloading) return;
	if (shooting) stopShooting();
	reloadingStartTime = utils::now();
	reloading = true;
}
