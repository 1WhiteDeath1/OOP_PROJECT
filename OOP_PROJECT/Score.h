#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
using namespace sf;

// keeps the player's score. kills made quickly one after another build up a combo that
// multiplies the points (up to x5). every scoring kill also shows a small "+100" in the world.
// the best score ever is kept in highscore.txt
class Score
{
	int points = 0;
	int combo = 0;          // kills in the current combo
	float comboTimer = 0;   // seconds left to make the next kill and keep the combo going

	// floating "+points" texts where something was killed
	static const int MAX_POPUPS = 20;
	struct Popup { float x, y, timer; int value; };
	Popup popups[MAX_POPUPS];

public:
	static constexpr float COMBO_TIME = 2.5f;

	Score(int startPoints = 0);
	void add(int basePoints, float worldX, float worldY); // a kill / rescue worth basePoints
	void update(float dt);
	void renderPopups(RenderWindow& w, const Camera& cam, Text& text);

	int getPoints() const { return points; }
	int getMultiplier() const { return combo < 1 ? 1 : (combo > 5 ? 5 : combo); }
	bool comboActive() const { return comboTimer > 0 && combo > 1; }

	// best score, read from / written to highscore.txt next to the game
	static int loadHighScore();
	static void saveHighScore(int value); // only writes if value beats the old best
};
