#include "player.hpp"

#include "game_level.hpp"
#include <cmath>
#include "game_manager.hpp"
#include "overhead_bar.cpp"
#include "utils.hpp"
#include "grenade.hpp"
#include "gun_drop.hpp"

Player::Player(GameManager *_manager, GameEntity *parent) : Player(_manager, parent, sf::Vector2f(0, 0)) {}
Player::Player(GameManager *_manager, GameEntity *parent, float x, float y) : Player(_manager, parent, sf::Vector2f(x, y)) {}
Player::Player(GameManager *_manager, GameEntity *parent, sf::Vector2f pos) : Character(_manager, parent, pos) {
	setPosition(pos);
	setSize(200, 400);
	maxHealth = 100.0f;
	health = 100.0f;
	
	maxAmmo = 200;
	ammo = 100;
	
	gun1 = Gun::createShotgun(_manager, this, 3);
	// addChild(gun1);
	
	healthBar = new OverheadBar<float>(_manager, this);
	healthBar->setCurrent(&health);
	healthBar->setMax(&maxHealth);
	healthBar->setOffset(30.0f);
	// addChild(healthBar);

	ammoBar = new OverheadBar<int>(_manager, this);
	ammoBar->setCurrent(&ammo);
	ammoBar->setMax(&maxAmmo);
	// ammoBar->setExtra(&gun->getCapacity());
	ammoBar->setOffset(60.0f);
	// addChild(ammoBar);

	animation_count = 0;
	//load all texture from resourceManager
	texture = new sf::Texture[18];
	for (int i = 0; i < 18; i++)texture[i] = manager->getTexture(PLAYER_TEXTURE, i);
	gunTexture = new sf::Texture[2];
	gunTexture[0] = manager->getTexture(GUN_TEXTURE, 2);
	gunTexture[1] = manager->getTexture(GUN_TEXTURE, 3);
}

GameLevel* Player::getTargetLevel() {return targetLevel;}
void Player::setTargetLevel(GameLevel *lvl) {targetLevel = lvl;}

void Player::draw(sf::RenderTarget &renderer) const {

	if ((*gun_ptr)) (*gun_ptr)->draw(renderer);

	//build, set and draw a sprite genarated from texture which was loaded before 
	sf::Sprite sprite(texture[current_motion]);
	sprite.setScale(getSize().x / texture[current_motion].getSize().x, getSize().y / texture[current_motion].getSize().y);
	sprite.setPosition(getTotalPosition() - getSize()/2.0f);
	renderer.draw(sprite);

	sf::Sprite sprite2(gunTexture[dir]);
	sprite2.setPosition(getTotalPosition().x + gun_arm_offsetX[dir], getTotalPosition().y + gun_arm_offsetY[dir]);
	sprite2.setScale(getSize().x / texture[dir].getSize().x, getSize().y / texture[dir].getSize().y);
	sprite2.setRotation(gun_angle_offset[dir]);
	if(current_motion > 1 && current_motion < 18 && (*gun_ptr)->getGunType()) renderer.draw(sprite2);

	if (DISPLAY_DEBUGGING_STUFF) {
		getCollisionBox().draw(renderer);
	}
}

void Player::update() {
	if (health < 0) return;

	Gun *gun = *gun_ptr;
	// Prepare collision boxes to do the checks against
	std::vector<CollisionBox> cboxes = manager->activeLevel->getBlockCollisionBoxes();
	
	userControllerVelocity = sf::Vector2f();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))userControllerVelocity += sf::Vector2f(-33, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))userControllerVelocity += sf::Vector2f(33, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))userControllerVelocity *= 0.2f;

	// Quick grounded check and jump
	if (getVelocity().y == 0.0f) {
		setCollisionBox(CollisionBox(getTotalPosition() + sf::Vector2f(0.0f, 1.0f), getSize()));
		if (checkObstruction(cboxes))
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
					addVelocity(sf::Vector2f(0, -120));
		setCollisionBox(CollisionBox(getTotalPosition(), getSize()));
	}
	int speed_count = 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))speed_count = 25;

	//Setting the motion of player in this moment
	if (userControllerVelocity.x > 0 && !gun->get_shooting() && !(utils::now() - gun->getLastShootingTime() < 100))
	{
		if (current_motion < move_right1 || current_motion > move_right8)
		{
			current_motion = move_right1;
			animation_count = 0;
			dir = 1;
		}
		animation_count++;
		if (animation_count > speed_count)
		{
			animation_count = 0;
			current_motion = (CHARACTER_MOTION)((int)current_motion + 1);
			if (current_motion > move_right8) current_motion = move_right1;
			dir = 1;
		}
	}
	else if (userControllerVelocity.x < 0 && !gun->get_shooting() && !(utils::now() - gun->getLastShootingTime() < 100))
	{
		if (current_motion < move_left1 || current_motion > move_left8)
		{
			current_motion = move_left1;
			animation_count = 0;
			dir = 0;
		}
		animation_count++;
		if (animation_count > speed_count)
		{
			animation_count = 0;
			current_motion = (CHARACTER_MOTION)((int)current_motion + 1);
			if (current_motion > move_left8) current_motion = move_left1;
			dir = 0;
		}
	}
	else
	{
		if ((manager->activeLevel->getCursorPosition() - getTotalPosition()).x < 0) current_motion = stand_left;
		else current_motion = stand_right;
	}
	

	//cerr << "current_motion:" << (int)character_motion << " SizeX:" << texture[character_motion].getSize().x << " SizeY:" << texture[character_motion].getSize().y << endl;
	
	addExternalImpact(userControllerVelocity);
	
	processBasicPhysics(cboxes);
	
	
	if (shouldSwitchGun) {
		shouldSwitchGun = false;
		
		// If we actually have 2 guns
		if (gun2 != nullptr) {
			(*gun_ptr)->detachFromParent();
			if (gun_ptr == &gun1) gun_ptr = &gun2;
			else gun_ptr = &gun1;
			cerr << "Switched gun" << endl;
			(*gun_ptr)->attachToParent(this);
		} else {
			cerr << "Attempted switching gun" << endl;
		}
	}
	
	if (shouldThrowGrenade) {
		shouldThrowGrenade = false;
		
		if (grenadeCount <= 0) return;
		grenadeCount--;
		
		Grenade *g = new Grenade(manager, manager->activeLevel);
		g->setPosition( getTotalPosition() + 100.0f * utils::normalize(manager->activeLevel->getCursorPosition() - getTotalPosition()) );
		g->setVelocity( 100.0f * utils::normalize(manager->activeLevel->getCursorPosition() - getTotalPosition()) );
		// manager->activeLevel->addChild(g);
	}
	
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (gun) gun->startShooting();
	} else {
		if (gun) gun->stopShooting();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) if (gun) gun->reload();
	
	if (gun) {
		//gun->setPosition(position);
		gun->setDirection(manager->activeLevel->getCursorPosition() - getTotalPosition());
		//gun->update();
	}
	
	if (gun_ptr == &gun1) {
		//cerr << "Current gun: 1" << endl;
	} else if (gun_ptr == &gun2) {
		//cerr << "Current gun: 2" << endl;
	} else {
		cerr << "Gun pointer error" << endl;
		exit(-1);
	}
	
	////cerr << "Player position: " << position.x << ", " << position.y << endl;
	////cerr << "Player velocity: " << velocity.x << " " << velocity.y << endl;
	////cerr << "Player health: " << health << endl;
}

void Player::receiveGun(Gun *g) {
	// If we have only 1 gun
	if (gun2 == nullptr) {
		//g->attachToParent(this);
		// addChild(g);
		gun2 = g;
		shouldSwitchGun = true;
	} else {
		// Switch currently active gun
		Gun *toDrop = *gun_ptr;
		toDrop->detachFromParent();
		//removeChild(toDrop);
		// addChild(g);
		g->attachToParent(this);
		*gun_ptr = g;
		
		//delete toDrop;
		new GunDrop(manager, manager->activeLevel, getTotalPosition(), toDrop);
	}
}
void Player::receiveAmmo(int a) {
	ammo = std::min(maxAmmo, ammo + a);
}
void Player::receiveHealth(int h) {
	health = std::min(maxHealth, health + h);
}

void Player::teleport(GameLevel *destLevel, sf::Vector2f pos) {
	setTargetLevel(destLevel);
	setPosition(pos);
}