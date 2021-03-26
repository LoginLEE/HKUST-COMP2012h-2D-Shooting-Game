#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <mutex>
#include <random>

#include <SFML/Graphics.hpp>

#include "global_defines.hpp"
#include "game_manager.hpp"
#include "game_level.hpp"
#include "player.hpp"

GameManager *game;
std::mutex m;
bool runUptader = true;

namespace utils {
	typedef std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31, 
	                             0x9908b0df, 11, 
	                             0xffffffff, 7, 
	                             0x9d2c5680, 15, 
	                             0xefc60000, 18, 1812433253> mt19937;
	mt19937 rng(100);
};

bool DISPLAY_DEBUGGING_STUFF = false;
int CURRENT_TICK = 0;

void updaterThread() {
	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
    while (runUptader) {
		// Dirty sync
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		int shouldWaitMicro = MICROSECONDS_PER_UPDATE - ( std::chrono::duration_cast<std::chrono::microseconds>(now - last) ).count();
		if (shouldWaitMicro > 1500) std::this_thread::sleep_for(std::chrono::microseconds(shouldWaitMicro-1500));
		volatile int dummy = 0;
		while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last).count() < MICROSECONDS_PER_UPDATE)
			dummy++;
		last = std::chrono::high_resolution_clock::now();
		
		// Catch up for skipped updates
		int missedUpdates = (-shouldWaitMicro) / MICROSECONDS_PER_UPDATE;
		while (missedUpdates > 0) {
			cerr << "There were " << missedUpdates << " missed updates." << endl;
			m.lock();
			game->update();
			m.unlock();
			missedUpdates--;
		}
		
		// Run current game iteration
		m.lock();
		game->update();
		m.unlock();
	}
}

int main(int argc, char **argv) {
	// Create a window with the best(?) mode;
	std::vector<sf::VideoMode> videoModes = sf::VideoMode::getFullscreenModes();	
	int windowWidth = videoModes[0].width;
	int windowHeight = videoModes[0].height;
	//sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "TBD", sf::Style::Fullscreen);
	windowWidth = 1920;
	windowHeight = 1080;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "TBD");
		
	// The one and only
	game = new GameManager(window, videoModes[0].width, videoModes[0].height);
	
	// Game will be updated in another thread
	std::thread updater(updaterThread);
	
	// The current thread will be rendering with Vsync
	window.setVerticalSyncEnabled(true);
	// (Drawing should be in the thread with the window due to a library requirement)
	
    while (window.isOpen()) {
		
		// Lock mutex for safety
		m.lock();
		
		// Event processing (in the thread with the window due to library requirement)
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				runUptader = false;
				m.unlock();
				break;
			}
			// ...
			else game->processEvent(event);
		}
		
		// Check if window still exists before drawing
		if (!window.isOpen()) break;
		
		window.clear(sf::Color::Cyan);
		game->draw();
		
		// Unlock mutex here so that the other thread can work while we wait for Vsync in window.display()
		m.unlock();
		
		// This might take a long time due to Vsync
		window.display();
	}
	
	// Wait for other thread to end
	updater.join();
	
	return 0;
}