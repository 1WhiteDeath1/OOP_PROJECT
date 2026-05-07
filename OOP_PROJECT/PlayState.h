#pragma once
#include "GameState.h"
#include "GameStateManager.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class PlayState : public GameState {
	GameStateManager& gsManager;
public:
	PlayState(GameStateManager& gsm) : gsManager(gsm) {}
	void enter()           override {}
	void exit()            override {}
	void handleInput()     override {}
	void update(float dt)  override {}
	void render(RenderWindow& w) override {}
};
