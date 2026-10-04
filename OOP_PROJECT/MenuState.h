#pragma once
#include "GameState.h"
#include "GameStateManager.h"
#include <SFML/Graphics.hpp>
using namespace sf;
class MenuState: public GameState
{
	GameStateManager& gsManager;

	Texture bgTex;
	Sprite bgSprite;

	Texture survivalTex, exitTex;
	Sprite survivalSprite, exitSprite;
	
	int choice = 0;  // 0=survival, 1=exit
	bool keyHeld = true; // wait for keys to be released first (e.g. coming back from a game)
public:
	MenuState(GameStateManager& gsm);
	void enter()   override;
	void exit()    override {}
	void handleInput() override;
	void update(float dt) override {}
	void render(RenderWindow& w) override;
};

