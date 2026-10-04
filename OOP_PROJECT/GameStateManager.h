#pragma once
#include "GameState.h"
class GameStateManager
{
	GameState* curr = nullptr;
	GameState* pending = nullptr; // state switches wait for the next frame so a state never deletes itself mid-update
	bool quit = false;
public:
	void changeState(GameState* nState);
	void applyPending();
	void handleInput();
	void update(float dt);
	void render(sf::RenderWindow& w);
	void requestQuit() { quit = true; }
	bool shouldQuit() const { return quit; }
	~GameStateManager();

};
