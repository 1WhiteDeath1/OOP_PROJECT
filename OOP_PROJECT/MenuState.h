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
	Font font;
	Text highScoreText;
	Text aimText; // the aim option line
	Text hintText; // the keys at the bottom
	bool tabHeld = false;

	float clock = 0;               // seconds since the menu opened (for the pulse)
	float fade = 1;                // black fade-in when the menu opens, 1 = black, 0 = clear
	float scales[2] = { 0.1f, 0.1f }; // the buttons grow smoothly instead of jumping
	Vector2i lastMouse;            // the mouse only picks a button when it moves (so it doesn't fight the keys)
	bool clickHeld = true;
	void choose();                 // start the game or quit
	Sprite survivalSprite, exitSprite;
	
	int choice = 0;  // 0=survival, 1=exit
	bool keyHeld = true; // wait for keys to be released first (e.g. coming back from a game)
public:
	MenuState(GameStateManager& gsm);
	void enter()   override;
	void exit()    override {}
	void handleInput() override;
	void update(float dt) override;
	void render(RenderWindow& w) override;
};

