#pragma once

//#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <limits>

#define MICROSECONDS_PER_UPDATE 16666

// In terms of blocks
#define ROOM_WIDTH 25
#define ROOM_HEIGHT 25

#define BACKGROUND_SIZE 10000.0f

#define BLOCK_SIZE 300.0f

// In addition to object's own size
#define TELEPORTER_SPAWN_OFFSET 50.0f

#define MAX_POWER_LEVEL 10.0f

#define TEXTURE_PATH (std::string("./texture/"))
#define MAP_PATH (std::string("./map.txt"))

#define ENEMY_RADIUS ((float)100)
#define ENEMY_Y_STRETCH 2.0

#define GUN_DROP_COLLECT_DISTANCE (BLOCK_SIZE * 1.5f)

//#define DOWNSCALED_WIDTH 1920.0f
//#define DOWNSCALED_HEIGHT 1080.0f

enum MapElement {
	MENone, METexturedBlock, MEColoredBlock, METeleporter, MEEnemy, MEPathMarker 
};

extern int CURRENT_TICK;

// For debugging
extern bool DISPLAY_DEBUGGING_STUFF;

#include <iostream>
using std::cerr; using std::endl;

//#ifdef DISPLAY_DEBUGGING_STUFF
//#pragma message "Rendering Debugging Stuff!"
//#endif