#pragma once
#include <SFML/Graphics.hpp>

class GameState {
public:
	virtual void handleInput() = 0;
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual ~GameState() = default;
};