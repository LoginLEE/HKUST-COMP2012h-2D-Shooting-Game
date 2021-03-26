#include "enemy.hpp"
#include "game_manager.hpp"
#include "health_drop.hpp"
#include "ammo_drop.hpp"
#include "overhead_bar.cpp"
#include "gun.hpp"
#include "utils.hpp"
#include "collision_ray.hpp"

Enemy::Enemy(GameManager *_manager, GameEntity *parent = nullptr) : Enemy(_manager, parent, sf::Vector2f(0, 0)) {}
Enemy::Enemy(GameManager *_manager, GameEntity *parent, float x, float y) : Enemy(_manager, parent, sf::Vector2f(x, y)) {}
Enemy::Enemy(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : Character(_manager, parent, pos) {
	setSize(ENEMY_RADIUS*2, ENEMY_RADIUS*2*ENEMY_Y_STRETCH);

	maxHealth = 10.0f;
	health = 10.0f;

	gun = Gun::createPistol(_manager, this, 10);
	// addChild(gun);
	gun->setDirection(utils::vectorFromDegrees(utils::randomf()*360.0f));
	
	healthBar = new OverheadBar<float>(_manager, this);
	healthBar->setCurrent(&health);
	healthBar->setMax(&maxHealth);
	//healthBar->setOffset(10.0f);
	// addChild(healthBar);

	// setVelocity(utils::randomv2()*200.0f);

	texture = new sf::Texture[2];
	for (int i = 0; i < 2; i++)texture[i] = manager->getTexture(ENEMY_TEXTURE, i);
	current_motion = stand_left;

	

}

Character* Enemy::getTarget() {return target;}
void Enemy::setTarget(Character *tgt) {target = tgt;}

void Enemy::draw(sf::RenderTarget &renderer) const {
	/*
	sf::CircleShape shape(ENEMY_RADIUS);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(getTotalPosition());
	shape.setScale(1.0, ENEMY_Y_STRETCH);
	shape.setOrigin(ENEMY_RADIUS, ENEMY_RADIUS);
	renderer.draw(shape);
	*/

	// cerr << "enemy size :" << getSize().x << " " << getSize().y << endl;

	sf::Sprite sprite(texture[current_motion]);
	sprite.setScale(getSize().x / texture[current_motion].getSize().x * 1.5f, getSize().y / texture[current_motion].getSize().y);
	sprite.setPosition(getTotalPosition().x - getSize().x / 2.0f, getTotalPosition().y - getSize().y / 2.0f);
	renderer.draw(sprite);

	if (DISPLAY_DEBUGGING_STUFF) {
		getCollisionBox().draw(renderer);
		CollisionRay::fromTwoPoints(getCollisionBox().getCenter(), target->getCollisionBox().getCenter()).draw(renderer);
		CollisionBox(getFeetPos(), sf::Vector2f(50.0, 50.0)).draw(renderer, sf::Color::Blue);
	}
	if (gun) gun->draw(renderer);
}

void Enemy::update() {
	// Prepare collision boxes to do the checks against
	std::vector<CollisionBox> blocks;
	std::vector<CollisionBox> enemies;

	// Find and sort Entities for various checks
	for (GameEntity *entity : manager->activeLevel->getEntities()) {
		if (Player *player = dynamic_cast<Player*>(entity)) {
			setTarget(player);
		} else if (const Enemy* en = dynamic_cast<const Enemy*>(entity)) {
			if (en == this) continue;
			enemies.push_back(en->getCollisionBox());
		} else if (const Block *block = dynamic_cast<const Block*>(entity)) {
			blocks.push_back(block->getCollisionBox());
		} else if (PathMarker *pathMarker = dynamic_cast<PathMarker*>(entity)) {
			if (pathMarker->steppingIn(this)) {
				currentPath = pathMarker;
			}
		}
	}
	
	processBasicPhysics(manager->activeLevel->getBlockCollisionBoxes());

	if (currentPath) {
		// cerr << "Enemy Pathing";
		switch (currentPath->getPathDir()) { // I Like Switch Statements ;)
			case PathMarker::Direction::EnemyPathLeft:
				addExternalImpact(currentPath->getSpeed() * sf::Vector2f(-10.0f, 0.0f));
				// cerr << " <- Left" << endl;
				break;
			case PathMarker::Direction::EnemyPathRight:
				addExternalImpact(currentPath->getSpeed() * sf::Vector2f(10.0f, 0.0f));
				// cerr << " -> Right" << endl;
				break;
			case PathMarker::Direction::EnemyPathJumpLeft:
				addExternalImpact(currentPath->getSpeed() * sf::Vector2f(-10.0f, -50.0f));
				// cerr << " <- JumpLeft" << endl;
				break;
			case PathMarker::Direction::EnemyPathJumpRight:
				addExternalImpact(currentPath->getSpeed() * sf::Vector2f(10.0f, -50.0f));
				// cerr << " -> JumpRight" << endl;
				break;
			case PathMarker::Direction::EnemyPathIdle:
				// cerr << " -- Idle" << endl;
				resetExternalImpact();
				break;
		}
	}

	// Gun aiming and Shooting
	if (target != nullptr && (currentPath == nullptr || currentPath->shouldShoot())) {
		CollisionBox targetBox = target->getCollisionBox();
		CollisionRay playerRay = CollisionRay::fromTwoPoints(getCollisionBox().getCenter(), targetBox.getCenter());
		float playerRayAngle = playerRay.getAngle();
		
		
		bool allowedAngle = std::min(std::abs(utils::normalizedAngleDiff(playerRayAngle, 0)), std::abs(utils::normalizedAngleDiff(playerRayAngle, 180.0f))) < 45.0f;
		if (foundTargetCounter > 0.0f) allowedAngle = true; // If already being seen, continue that way
		
		if (allowedAngle && !playerRay.getFirstIntersection(blocks, targetBox)) {
			// If nothing between Enemy and the Player
			// Aka line of sight
			float angleDiff = utils::vectorAngleDiff(target->getTotalPosition() - getTotalPosition(), gun->getDirection());
			gun->rotateBy(angleDiff * utils::randomf()/5); //random P controller lol
			// cerr << "enemy target angle: " << angleDiff << endl;
			//if (foundTargetCounter == -1) foundTargetCounter = utils::now();
			foundTargetCounter = std::min(6.0f, foundTargetCounter + 1.0f);
		} else {
			float angleDiff = utils::vectorAngleDiff(getExternalImpact(), gun->getDirection());
			if (foundTargetCounter < 1.0f) gun->rotateBy(angleDiff * utils::randomf()/5); //random P controller lol
			foundTargetCounter = std::max(0.0f, foundTargetCounter - 0.06f);
			// cerr << "player sight blocked" << endl;
		}

		if (foundTargetCounter == 6.0f) {
			CollisionRay ray = gun->getShootingRay();
			if (ray.checkIntersection(target->getCollisionBox())) {
				// gun->startShooting();
				CollisionBox closest = target->getCollisionBox();
				if (!ray.getFirstIntersection(blocks, closest) && !ray.getFirstIntersection(enemies, closest)) {
					// If there is no block or enemy in the line of fire
					gun->startShooting();
					
					// Stop while shooting
					resetExternalImpact();
				} else {
					gun->stopShooting();
				}
			} else gun->stopShooting();
		} else {
			gun->stopShooting();
		}

		//change enemy texture motion
		if (!gun->getDir()) current_motion = stand_left;
		else current_motion = stand_right;
	} else {
		gun->stopShooting();
	}
	
	if (health <= 0) {
		Collectible::createCollectibleWithPower(manager, manager->activeLevel, getTotalPosition(), manager->activeLevel->getDifficulty());
		// manager->activeLevel->addChild(new Collectible(manager, manager->activeLevel, getTotalPosition(), 40.0f));
		setAlive(false);
	}

}