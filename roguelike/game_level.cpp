#include "game_level.hpp"
#include "game_manager.hpp"

GameLevel::GameLevel(GameManager *_manager, float difficulty, sf::RenderWindow &r, sf::View &v) 
	: GameEntity(_manager, nullptr), difficulty(difficulty), renderWindow(r), view(v), player(nullptr) {
	//generate();
}
GameLevel::~GameLevel() {
	// nothing additional
}

float GameLevel::getDifficulty() const {
	return difficulty;
}

std::vector<const GameEntity*> GameLevel::getEntities() const {
	return getSubtree();
}
std::vector<GameEntity*> GameLevel::getEntities() {
	return getSubtree();
}

std::vector<CollisionBox> GameLevel::getBlockCollisionBoxes() const {
	std::vector<CollisionBox> ret;
	for (const GameEntity *entity : getSubtree()) {
		if (const Block *block = dynamic_cast<const Block*>(entity)) {
			ret.push_back(block->getCollisionBox());
		}
	}
	return ret;
}
std::vector<CollidingEntity*> GameLevel::getCollidingEntities(CollisionBox cbox) {
	std::vector<CollidingEntity*> ret;
	for (GameEntity *entity : getSubtree()) {
		if (CollidingEntity *ce = dynamic_cast<CollidingEntity*>(entity)) {
			if (ce->collidesWith(cbox)) ret.push_back(ce);
		} 
	}
	return ret;
}

void GameLevel::update() {
	
	//cerr << "Number of entities: " << getSubtree().size() << endl;
	
	// Update cursor position
	cursorPosition = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
	cursorPosition.x = std::max(cursorPosition.x, view.getCenter().x - view.getSize().x/2.0f);
	cursorPosition.x = std::min(cursorPosition.x, view.getCenter().x + view.getSize().x/2.0f);
	cursorPosition.y = std::max(cursorPosition.y, view.getCenter().y - view.getSize().y/2.0f);
	cursorPosition.y = std::min(cursorPosition.y, view.getCenter().y + view.getSize().y/2.0f);
	
	// Update camera	
	sf::Vector2f cameraCenter = (3.0f*manager->getPlayer()->getTotalPosition() + cursorPosition) / 4.0f;
	view.setCenter(cameraCenter);
	
	// Level transition
	//if (player->getPosition().x < 400 && leftLevel) {player->setTargetLevel(leftLevel);}
	//if (player->getPosition().x > 6000 && rightLevel) {player->setTargetLevel(rightLevel);}
	
}

sf::Vector2f GameLevel::getCursorPosition() const {return cursorPosition;}
GameLevel* GameLevel::getLeftLevel() {return leftLevel;}
GameLevel* GameLevel::getRightLevel() {return rightLevel;}
GameLevel* GameLevel::getUpLevel() {return upLevel;}
GameLevel* GameLevel::getDownLevel() {return downLevel;}
void GameLevel::setLeftLevel(GameLevel *lvl) {leftLevel = lvl;}
void GameLevel::setRightLevel(GameLevel *lvl) {rightLevel = lvl;}
void GameLevel::setUpLevel(GameLevel *lvl) {upLevel = lvl;}
void GameLevel::setDownLevel(GameLevel *lvl) {downLevel = lvl;}
Teleporter* GameLevel::getLeftTeleporter() {return leftTeleporter;}
Teleporter* GameLevel::getRightTeleporter() {return rightTeleporter;}
Teleporter* GameLevel::getUpTeleporter() {return upTeleporter;}
Teleporter* GameLevel::getDownTeleporter() {return downTeleporter;}
void GameLevel::setLeftTeleporter(Teleporter *tp) {leftTeleporter = tp;}
void GameLevel::setRightTeleporter(Teleporter *tp) {rightTeleporter = tp;}
void GameLevel::setUpTeleporter(Teleporter *tp) {upTeleporter = tp;}
void GameLevel::setDownTeleporter(Teleporter *tp) {downTeleporter = tp;}