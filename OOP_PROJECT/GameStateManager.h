#pragma once
#include "GameState.h"
class GameStateManager
{
	GameState* curr = nullptr;
public:
	void changeState(GameState* nState);
	void handleInput();
	void update(float dt);
	void render(sf::RenderWindow& w);
	~GameStateManager();

};

