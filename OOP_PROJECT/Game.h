#pragma once
#include <SFML/Graphics.hpp>
#include "GameStateManager.h"
class Game
{
	sf::RenderWindow window;
	GameStateManager stateManager;
	sf::Clock clock;
public:
	Game();
	void run();
	GameStateManager& getStateManager() { return stateManager; }
	sf::RenderWindow& getWindow() { return window; }

};

