#pragma once
#include "GameState.h"
#include "GameStateManager.h"
#include <SFML/Graphics.hpp>
using namespace sf;
class MenuState: public GameState
{
	GameStateManager& gsManager;
	Font font;
	Text title, option1, option2;
	int choice = 0;  // 0=survival, 1=campaign
	bool keyHeld = false;
public:
	MenuState(GameStateManager& gsm);
	void enter()   override;
	void exit()    override {}
	void handleInput() override;
	void update(float dt) override {}
	void render(RenderWindow& w) override;
};

