#include "game_manager.hpp"
#include <fstream>
#include "utils.hpp"
#include "path_marker.hpp"
#include "gun_drop.hpp"

GameManager::GameManager(sf::RenderWindow &rw, int renderWidth, int renderHeight) : renderWindow(rw) {
	
	CURRENT_TICK = 0;
	
	view.reset(sf::FloatRect(0, 0, 5760, 3240));
	renderWindow.setView(view);
	resourceManager.LoadAllResource();

	loadFromMap(MAP_PATH);
	
	hud = new HUD(this, nullptr);
		
	/*player = new Player(this, nullptr, sf::Vector2f(3200, 500));
	
	// Hardcode
	GameLevel *testLvl = new GameLevel(this, 1, renderWindow, view);
	levels.push_back(testLvl);
	
	GameLevel *testLvl2 = new GameLevel(this, 2, renderWindow, view);
	levels.push_back(testLvl2);	
	
	testLvl->setRightLevel(testLvl2);
	testLvl2->setLeftLevel(testLvl);
	
	activeLevel = testLvl;
	player->attachToParent(activeLevel);
	// player->setCurrentLevel(testLvl);
	
	Teleporter *tp1 = new Teleporter(this, testLvl, 20*BLOCK_SIZE, BLOCK_SIZE*(ROOM_HEIGHT-2));
	tp1->setLevel(testLvl);
	Teleporter *tp2 = new Teleporter(this, testLvl2, 2*BLOCK_SIZE, BLOCK_SIZE*(ROOM_HEIGHT-3));
	tp2->setLevel(testLvl2);
	
	tp1->setAssociate(tp2);
	tp2->setAssociate(tp1);
	
	// testLvl->addChild(tp1);
	// testLvl2->addChild(tp2);*/
}

GameManager::~GameManager() {
	delete player;
	for (GameLevel* lvl : levels) delete lvl;
}

void GameManager::loadFromMap(std::string map_path) {
	using std::pair;
	using std::make_pair;
	std::ifstream in(map_path);
	if (in.fail()) utils::panic("There was a problem opening map file");
	int numMaps; in >> numMaps;
	std::vector<pair<pair<int, int>, GameLevel*> > maps;
	for (int numMapsCounter = 0; numMapsCounter < numMaps; numMapsCounter++) {
		int mapCoordinateX, mapCoordinateY; in >> mapCoordinateX >> mapCoordinateY;
		GameLevel *level = new GameLevel(this, 1 + mapCoordinateX + mapCoordinateY, renderWindow, view);
		maps.push_back(make_pair(make_pair(mapCoordinateX, mapCoordinateY), level));
		levels.push_back(level);
		int mapHeight, mapWidth; in >> mapHeight >> mapWidth;
		//background test
		/////////////////////////
		Background *bg = new Background(this, level, BACKGROUND_SIZE, BACKGROUND_SIZE);
		// level->addChild(bg);
		/////////////////////////
		std::vector<pair<int, Teleporter*> > inLevelTeleporters;
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				int a, b; in >> a >> b;
				// I hate switch case
				if (a == MapElement::MENone) {
					// nothing to do
				} else if (a == MapElement::METexturedBlock) {
					TexturedBlock *tb = new TexturedBlock(this, static_cast<BLOCK_TYPE>(b), level, j*BLOCK_SIZE, i*BLOCK_SIZE);
					// level->addChild(tb);
				} else if (a == MapElement::MEColoredBlock) {
					ColoredBlock *cb = new ColoredBlock(this, ColoredBlock::blockColorMap(b), level, j*BLOCK_SIZE, i*BLOCK_SIZE);
					// ColoredBlock *cb = new ColoredBlock(this, sf::Color::Yellow, level, j*BLOCK_SIZE, i*BLOCK_SIZE);
					// level->addChild(cb);
				} else if (a == MapElement::METeleporter) {
					Teleporter *tp = new Teleporter(this, level, j*BLOCK_SIZE, i*BLOCK_SIZE);
					tp->setLevel(level);
					// level->addChild(tp);
					if (b == Teleporter::Type::PlayerSpawn) {
						player = new Player(this, nullptr);
						player->setPosition(j*BLOCK_SIZE, i*BLOCK_SIZE - TELEPORTER_SPAWN_OFFSET - player->getSize().y);
						player->attachToParent(level);
						activeLevel = level;
						view.setCenter(player->getTotalPosition());
					} else if (b == Teleporter::Type::UpTeleporter) {
						level->setUpTeleporter(tp);
					} else if (b == Teleporter::Type::DownTeleporter) {
						level->setDownTeleporter(tp);
					} else if (b == Teleporter::Type::LeftTeleporter) {
						level->setLeftTeleporter(tp);
					} else if (b == Teleporter::Type::RightTeleporter) {
						level->setRightTeleporter(tp);
					} else {
						inLevelTeleporters.push_back(make_pair(b, tp));
					}
				} else if (a == MapElement::MEEnemy) {
					Enemy *en = new Enemy(this, level);
					en->setPosition(j*BLOCK_SIZE, i*BLOCK_SIZE - TELEPORTER_SPAWN_OFFSET);
					// level->addChild(en);
				} else if (a == MapElement::MEPathMarker) {
					PathMarker::create(this, b, level, j*BLOCK_SIZE, i*BLOCK_SIZE);
					// level->addChild(pathMarker);
				} else {
					utils::panic("Unknown map element while loading");
				}
			}
		}



		// Connect inlevel teleporters together
		for (int i = 0; i < inLevelTeleporters.size(); i++) {
			int id = inLevelTeleporters[i].first;
			Teleporter *tp = inLevelTeleporters[i].second;
			for (int j = i+1; j < inLevelTeleporters.size(); j++) {
				int id2 = inLevelTeleporters[j].first;
				Teleporter *tp2 = inLevelTeleporters[j].second;
				if (id == -id2) {
					tp->setAssociate(tp2);
					tp2->setAssociate(tp);
				}
			}
		}
		// Level borders
		for (int i = -1; i <= mapHeight; i++) {
			for (int j = -1; j <= mapWidth; j++) {
				if ( (i!=-1&&i!=mapHeight) && (j!=-1&&j!=mapWidth) ) continue;
				ColoredBlock *cb = new ColoredBlock(this, sf::Color::Yellow, level, j*BLOCK_SIZE, i*BLOCK_SIZE);
				// level->addChild(cb);
			}
		}
	}
	// Connect interlevel teleporters together
	for (std::vector<pair<pair<int, int>, GameLevel*> >::iterator it = maps.begin(); it != maps.end(); it++) {
		pair<int, int> coord = (*it).first;
		GameLevel *level = (*it).second;
		for (std::vector<pair<pair<int, int>, GameLevel*> >::iterator it2 = it; it2 != maps.end(); it2++) {
			pair<int, int> coord2 = (*it2).first;
			GameLevel *level2 = (*it2).second;
			if (coord.first == coord2.first && coord.second == coord2.second + 1) {
				level->setUpLevel(level2);
				level2->setDownLevel(level);
				Teleporter *tp = level->getUpTeleporter();
				Teleporter *tp2 = level2->getDownTeleporter();
				if (tp && tp2) {
					tp->setAssociate(tp2);
					tp2->setAssociate(tp);
				}
			} else if (coord.first == coord2.first && coord.second == coord2.second - 1) {
				level->setDownLevel(level2);
				level2->setUpLevel(level);
				Teleporter *tp = level->getDownTeleporter();
				Teleporter *tp2 = level2->getUpTeleporter();
				if (tp && tp2) {
					tp->setAssociate(tp2);
					tp2->setAssociate(tp);
				}
			} else if (coord.first == coord2.first + 1 && coord.second == coord2.second) {
				level->setLeftLevel(level2);
				level2->setRightLevel(level);
				Teleporter *tp = level->getLeftTeleporter();
				Teleporter *tp2 = level2->getRightTeleporter();
				if (tp && tp2) {
					tp->setAssociate(tp2);
					tp2->setAssociate(tp);
				}
			} else if (coord.first == coord2.first - 1 && coord.second == coord2.second) {
				level->setRightLevel(level2);
				level2->setLeftLevel(level);
				Teleporter *tp = level->getRightTeleporter();
				Teleporter *tp2 = level2->getLeftTeleporter();
				if (tp && tp2) {
					tp->setAssociate(tp2);
					tp2->setAssociate(tp);
				}
			}
		}
	}
	
}

void GameManager::updateNearestEntity() {
	selectedEntity = nullptr;
	nearestSelectableEntity = nullptr;
	float nearestSelectableDistance = std::numeric_limits<float>::max();
	for (GameEntity *entity : activeLevel->getSubtree()) {
		if (GunDrop *gd = dynamic_cast<GunDrop*>(entity)) {
			float distance = utils::distance(player->getTotalPosition(), gd->getTotalPosition());
			if (distance < nearestSelectableDistance) {
				nearestSelectableEntity = entity;
				nearestSelectableDistance = distance;
			}
		}
	}
	if (GunDrop *gd = dynamic_cast<GunDrop*>(nearestSelectableEntity)) {
		if (nearestSelectableDistance < GUN_DROP_COLLECT_DISTANCE) {
			selectedEntity = nearestSelectableEntity;
			gd->excite(2);
		}
	}
}

void GameManager::update() {
	
	CURRENT_TICK++;
	
	updateNearestEntity();
	if (activeLevel) {
		activeLevel->updateNode();
	}
	updateNearestEntity();
	
	// Level transition
	if (player->getTargetLevel() && player->getTargetLevel() != activeLevel) {
		GameLevel *currentLevel = activeLevel;
		GameLevel *destLevel = player->getTargetLevel();

		player->setTargetLevel(nullptr);

		player->detachFromParent();
		player->attachToParent(destLevel);
		
		activeLevel = destLevel;
	}
	
	if (hud) {
		hud->updateNode();
	}
	
	// This updates the actual view that is used
	//renderWindow.setView(view);
}

void GameManager::draw() {
	//renderWindow.clear(sf::Color::White);
	renderWindow.setView(view);
	if (activeLevel) {
		activeLevel->drawNode(renderWindow);
	}
	if (hud) {
		hud->drawNode(renderWindow);
	}
	
	// So sad that it takes longer than a frame
	/*
	sf::Texture windowContents;
	windowContents.create(renderWindow.getSize().x, renderWindow.getSize().y);
	windowContents.update(renderWindow);
	
	sf::Sprite windowContentsSprite(windowContents);
	windowContentsSprite.setPosition(view.getCenter());
	windowContentsSprite.setOrigin(windowContents.getSize().x / 2.0f, windowContents.getSize().y / 2.0f);
	windowContentsSprite.setScale(sf::Vector2f(DOWNSCALED_WIDTH / windowContents.getSize().x * view.getSize().x / windowContents.getSize().x, DOWNSCALED_HEIGHT / windowContents.getSize().y * view.getSize().y / windowContents.getSize().y));
	renderWindow.draw(windowContentsSprite);
	
	sf::Texture smallWindowContents;
	smallWindowContents.loadFromImage(renderWindow.capture(), sf::IntRect((renderWindow.getSize().x - DOWNSCALED_WIDTH) / 2, (renderWindow.getSize().y - DOWNSCALED_HEIGHT) / 2, DOWNSCALED_WIDTH, DOWNSCALED_HEIGHT));
	//smallWindowContents.create(DOWNSCALED_WIDTH, DOWNSCALED_HEIGHT);
	//smallWindowContents.update(renderWindow, (renderWindow.getSize().x - DOWNSCALED_WIDTH) / 2, (renderWindow.getSize().y - DOWNSCALED_HEIGHT) / 2);
	
	sf::Sprite smallSprite(smallWindowContents);
	smallSprite.setPosition(view.getCenter());
	smallSprite.setOrigin(smallWindowContents.getSize().x / 2.0f, smallWindowContents.getSize().y / 2.0f);
	smallSprite.setScale(sf::Vector2f(view.getSize().x / smallWindowContents.getSize().x, view.getSize().y / smallWindowContents.getSize().y));
	//renderWindow.clear(sf::Color::White);
	renderWindow.draw(smallSprite);
	*/
	
	//renderWindow.display();
}

void GameManager::processEvent(const sf::Event &event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Q) {
			player->shouldSwitchGun = true;
		}
		if (event.key.code == sf::Keyboard::K) {
			DISPLAY_DEBUGGING_STUFF = ! DISPLAY_DEBUGGING_STUFF;
		}
		if (event.key.code == sf::Keyboard::G) {
			player->shouldThrowGrenade = true;
		}
		if (event.key.code == sf::Keyboard::E) {
			if (GunDrop *gd = dynamic_cast<GunDrop*>(selectedEntity)) {
				gd->excite(4);
			}
		}
	}
}

Player* GameManager::getPlayer() const {return player;}