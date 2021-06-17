#include "game_manager.hpp"
#include "hud.hpp"
#include "utils.hpp"
#include "player.hpp"
#include "gun.hpp"
#include "gun_drop.hpp"

sf::Font HUD::entryFont;

float GUN_INFO_HEIGHT = 600.0f;
float GUN_MINI_INFO_HEIGHT = 200.0f;
int GUN_INFO_FONT_SIZE = 65;
int GUN_MINI_INFO_LV_SIZE = 100;
float GUN_INFO_RHS_OFFSET = 375.0f;
float GUN_INFO_ENTRY_HEIGHT = GUN_INFO_HEIGHT / 8.5f;

HUD::HUD(GameManager *_manager, GameEntity *parent) : GameEntity(_manager, parent) {
	entryFont.loadFromFile("./fonts/abel/Abel-Regular.ttf");
}

void HUD::update() {
	sf::Vector2f hudPosition = manager->getViewPosition() - manager->getViewSize() / 2.0f;
	hudPosition += sf::Vector2f(100.0f, 100.0f);
	hudPosition += 100.0f * (manager->activeLevel->getCursorPosition() - manager->getPlayer()->getTotalPosition()) / manager->getViewSize().x;
	setPosition(hudPosition);
}

void HUD::draw(sf::RenderTarget &renderer) const {
	
	Player *player = manager->getPlayer();
	
	float health = player->getHealth() < 0 ? 0 : player->getHealth();
	float maxHealth = player->getMaxHealth();
	float healthRatio = health / maxHealth;
	float healthDepletionRatio = 1.0f - healthRatio;
	
	int ammo = player->getAmmo();
	int maxAmmo = player->getMaxAmmo();
	float ammoRatio = (float)ammo / (float)maxAmmo;
	float ammoDepletionRatio = 1.0f - ammoRatio;
	
	Gun *currentGun = player->getCurrentGun();
	int cap = currentGun->getCapacity();
	int maxCap = currentGun->getMaxCapacity();
	float capRatio = (float)cap / (float)maxCap;
	float capDepletionRatio = 1.0f - capRatio;
	
	int grenades = player->getGrenadeCount();
	
	
	// Healthbar
	sf::Color healthColor = utils::interpolateColor(sf::Color::Red, sf::Color::Green, healthRatio);
	sf::RectangleShape healthBar;
	healthBar.setPosition(getTotalPosition() + sf::Vector2f(0.0f, 50.0f));
	healthBar.setSize(sf::Vector2f(10.0f + 20.0f * maxHealth, 100.0f));
	healthBar.setOrigin(0, healthBar.getSize().y / 2.0f);
	healthBar.setFillColor(sf::Color::Transparent);
	healthBar.setOutlineColor(healthColor);
	healthBar.setOutlineThickness(10.0f);
	renderer.draw(healthBar);
	sf::RectangleShape healthBarStick;
	healthBarStick.setPosition(getTotalPosition() + sf::Vector2f(5.0f, 50.0f));
	healthBarStick.setSize(sf::Vector2f(20.0f * health, 100.0f));
	healthBarStick.setOrigin(0, healthBarStick.getSize().y / 2.0f);
	healthBarStick.setFillColor(healthColor);
	renderer.draw(healthBarStick);
	
	// Ammobar
	sf::Color ammoColor = utils::interpolateColor(sf::Color(128, 128, 128), sf::Color(255, 128, 0), ammoRatio);
	sf::RectangleShape ammoBar;
	ammoBar.setPosition(getTotalPosition() + sf::Vector2f(0.0f, 200.0f));
	ammoBar.setSize(sf::Vector2f(10.0f + 20.0f * maxAmmo, 100.0f));
	ammoBar.setOrigin(0, healthBar.getSize().y / 2.0f);
	ammoBar.setFillColor(sf::Color::Transparent);
	ammoBar.setOutlineColor(ammoColor);
	ammoBar.setOutlineThickness(10.0f);
	renderer.draw(ammoBar);
	sf::RectangleShape ammoBarStick;
	ammoBarStick.setPosition(getTotalPosition() + sf::Vector2f(5.0f, 200.0f));
	ammoBarStick.setSize(sf::Vector2f(20.0f * ammo, 100.0f));
	ammoBarStick.setOrigin(0, healthBarStick.getSize().y / 2.0f);
	ammoBarStick.setFillColor(ammoColor);
	renderer.draw(ammoBarStick);
	
	// Grenades
	sf::Color grenadeColor = sf::Color(70, 70, 70);
	//sf::Vector2f grenadesStartPosition = ammoBar.getPosition() + sf::Vector2f(ammoBar.getSize().x, 0.0f) + sf::Vector2f(100.0f, 0.0f);
	sf::Vector2f grenadesStartPosition = ammoBar.getPosition() + sf::Vector2f(40.0f, 150.0f);
	for (int i = 0; i < grenades; i++) {
		sf::CircleShape grenade;
		grenade.setPosition(grenadesStartPosition + (float)i * sf::Vector2f(120.0f, 0.0f));
		grenade.setRadius(35.0f);
		grenade.setOrigin(sf::Vector2f(grenade.getRadius(), grenade.getRadius()));
		grenade.setFillColor(grenadeColor);
		grenade.setOutlineColor(sf::Color::Black);
		grenade.setOutlineThickness(10.0f);
		renderer.draw(grenade);
	}
	
	// Gun details
	utils::DrawableGroup currentGunDetails = craftGunInfo(currentGun);
	sf::Vector2f gunDetailsPosition = getTotalPosition();
	gunDetailsPosition += sf::Vector2f(0.0f, manager->getViewSize().y);
	gunDetailsPosition += sf::Vector2f(0.0f, -(GUN_INFO_HEIGHT + 200.0f));
	currentGunDetails.setPosition(gunDetailsPosition);
	currentGunDetails.draw(renderer);
	currentGunDetails.clear();
	
	if (GunDrop *gd = dynamic_cast<GunDrop*>(manager->getSelectedEntity())) {
		currentGunDetails = craftGunInfo(gd->getGun(), currentGun);
		gunDetailsPosition += sf::Vector2f(0.0f, -(GUN_INFO_HEIGHT + 50.0f));
		currentGunDetails.setPosition(gunDetailsPosition);
		currentGunDetails.draw(renderer);
		currentGunDetails.clear();
	}
	
	// CapBar
	//sf::Color capColor = utils::interpolateColor(sf::Color(128, 128, 128), sf::Color(255, 255, 0), capRatio);
	sf::Color capColor = sf::Color(128, 128, 128);
	sf::RectangleShape capBar;
	capBar.setPosition(gunDetailsPosition + sf::Vector2f(0.0f, -100.0f));
	capBar.setSize(sf::Vector2f(10.0f + 200.0f * maxCap, 100.0f));
	capBar.setOrigin(0, capBar.getSize().y / 2.0f);
	capBar.setFillColor(sf::Color::Transparent);
	capBar.setOutlineColor(capColor);
	capBar.setOutlineThickness(10.0f);
	renderer.draw(capBar);
	sf::RectangleShape capBarStick;
	capBarStick.setPosition(gunDetailsPosition + sf::Vector2f(5.0f, -100.0f));
	capBarStick.setSize(sf::Vector2f(200.0f * cap, 100.0f));
	capBarStick.setOrigin(0, capBarStick.getSize().y / 2.0f);
	capBarStick.setFillColor(capColor);
	renderer.draw(capBarStick);
	
	if (currentGun->isReloading()) {
		sf::Text relInd = sf::Text("RELOADING", entryFont, 100);
		relInd.setPosition(capBar.getPosition() + sf::Vector2f(capBar.getSize().x, -capBar.getSize().y / 2.0f) + sf::Vector2f(30.0f, -10.0f));
		renderer.draw(relInd);
	}
	
	if (Gun *secondaryGun = player->getSecondaryGun()) {
		currentGunDetails = craftGunMiniInfo(secondaryGun);
		gunDetailsPosition += sf::Vector2f(0.0f, -(capBar.getSize().y + 50.0f));
		gunDetailsPosition += sf::Vector2f(0.0f, -(GUN_MINI_INFO_HEIGHT + 50.0f));
		currentGunDetails.setPosition(gunDetailsPosition);
		currentGunDetails.draw(renderer);
		currentGunDetails.clear();
	}
}

utils::DrawableGroup HUD::craftGunInfo(Gun *gun, Gun *compareGun) {
	
	if (compareGun == nullptr) compareGun = gun;
	
	utils::DrawableGroup group;

	/*
	sf::RectangleShape *gunImage = new sf::RectangleShape; // Change this to sprite later
	gunImage->setSize(sf::Vector2f(GUN_INFO_HEIGHT, GUN_INFO_HEIGHT));
	gunImage->setFillColor(sf::Color(255, 0, 255));
	group.add(gunImage);
	*/

	sf::Texture *gunTexture = new sf::Texture;
	if(gun->getGunType())(*gunTexture).loadFromFile(TEXTURE_PATH + "gun/shotgun_icon.png");//*gunTexture = manager->getTexture(GUN_TEXTURE, 5);
	else (*gunTexture).loadFromFile(TEXTURE_PATH + "gun/pistol_icon.png");//*gunTexture = manager->getTexture(GUN_TEXTURE, 4);
	sf::Sprite *gunImage = new sf::Sprite;
	gunImage->setTexture(*gunTexture);
	gunImage->setScale(9.0f, 9.0f);
	group.add(gunImage);
	
	
	sf::Vector2f detailsPanePosition = sf::Vector2f(GUN_INFO_HEIGHT, 0.0f);
	sf::RectangleShape *detailsPaneBackground = new sf::RectangleShape;
	detailsPaneBackground->setPosition(sf::Vector2f(GUN_INFO_HEIGHT, 0.0f));
	detailsPaneBackground->setSize(sf::Vector2f(GUN_INFO_HEIGHT, GUN_INFO_HEIGHT));
	detailsPaneBackground->setFillColor(sf::Color(100, 100, 100));
	group.add(detailsPaneBackground);
	
	sf::Text *lhs, *rhs;
	int paneCounter = 0;
	sf::Vector2f entryPositionComponent = sf::Vector2f(10.0f, 5.0f);
	sf::Color betterColor = sf::Color::Green;
	sf::Color worseColor = sf::Color::Red;
	
	lhs = new sf::Text("Damage: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getDamage()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getDamage() > compareGun->getDamage()) rhs->setFillColor(betterColor);
	else if (gun->getDamage() < compareGun->getDamage()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	lhs = new sf::Text("Rate of fire: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getRof()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getRof() > compareGun->getRof()) rhs->setFillColor(betterColor);
	else if (gun->getRof() < compareGun->getRof()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	lhs = new sf::Text("Bullet speed: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getSpeed()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getSpeed() > compareGun->getSpeed()) rhs->setFillColor(betterColor);
	else if (gun->getSpeed() < compareGun->getSpeed()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	lhs = new sf::Text("Accuracy: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getAccuracy()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getAccuracy() > compareGun->getAccuracy()) rhs->setFillColor(betterColor);
	else if (gun->getAccuracy() < compareGun->getAccuracy()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	lhs = new sf::Text("Range: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getRange()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getRange() > compareGun->getRange()) rhs->setFillColor(betterColor);
	else if (gun->getRange() < compareGun->getRange()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	lhs = new sf::Text("Mag capacity: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getCap()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getCap() > compareGun->getCap()) rhs->setFillColor(betterColor);
	else if (gun->getCap() < compareGun->getCap()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	lhs = new sf::Text("Reload time: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getReload()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getReload() > compareGun->getReload()) rhs->setFillColor(betterColor);
	else if (gun->getReload() < compareGun->getReload()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	lhs = new sf::Text("Power Level: ", entryFont, GUN_INFO_FONT_SIZE);
	rhs = new sf::Text(utils::textify(gun->getPowerLevel()), entryFont, GUN_INFO_FONT_SIZE);
	if (gun->getPowerLevel() > compareGun->getPowerLevel()) rhs->setFillColor(betterColor);
	else if (gun->getPowerLevel() < compareGun->getPowerLevel()) rhs->setFillColor(worseColor);
	lhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(0.0f, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	rhs->setPosition(detailsPanePosition + entryPositionComponent + sf::Vector2f(GUN_INFO_RHS_OFFSET, paneCounter * GUN_INFO_ENTRY_HEIGHT));
	group.add(lhs);
	group.add(rhs);
	paneCounter++;
	
	return group;
}

utils::DrawableGroup HUD::craftGunMiniInfo(Gun *gun) {
	utils::DrawableGroup group;
	
	sf::RectangleShape *gunImage = new sf::RectangleShape; // Change also this to sprite later
	gunImage->setSize(sf::Vector2f(GUN_MINI_INFO_HEIGHT, GUN_MINI_INFO_HEIGHT));
	gunImage->setFillColor(sf::Color(128, 0, 128));
	group.add(gunImage);
	
	sf::Vector2f detailsPanePosition = sf::Vector2f(GUN_MINI_INFO_HEIGHT, 0.0f);
	sf::RectangleShape *detailsPaneBackground = new sf::RectangleShape;
	detailsPaneBackground->setPosition(sf::Vector2f(GUN_MINI_INFO_HEIGHT, 0.0f));
	detailsPaneBackground->setSize(sf::Vector2f(2*GUN_MINI_INFO_HEIGHT, GUN_MINI_INFO_HEIGHT));
	detailsPaneBackground->setFillColor(sf::Color(100, 100, 100));
	group.add(detailsPaneBackground);
	
	sf::Text *levelIndicator = new sf::Text(std::string("LV ") + utils::textify(gun->getPowerLevel()), entryFont, GUN_MINI_INFO_LV_SIZE);
	levelIndicator->setPosition(detailsPanePosition + sf::Vector2f(40.0f, 40.0f));
	group.add(levelIndicator);
	
	return group;
}
