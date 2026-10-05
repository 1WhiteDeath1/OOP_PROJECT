#pragma once
#include "GameState.h"
#include "GameStateManager.h"
#include "World.h"
#include "Player.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "Camera.h"
#include "DayNightCycle.h"
#include "Weather.h"
#include "Score.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class PlayState : public GameState {
	GameStateManager& gsManager;
	int mission;       // 1, 2 or 3
	World world;
	Player player;
	EntityManager entityManager;
	LevelManager levelManager;
	Camera camera;
	DayNightCycle dayNight;
	WeatherSystem weather;
	Score score;

	Font font;
	Text hudText;
	Text bannerText;
	Text scoreText;    // big score in the top right
	Text popupText;    // the floating "+100"s
	float endTimer = 0; // counts down after game over / mission complete before going back to the menu
	float introTimer = 3; // the mission title shows for the first 3 seconds
	bool missionDone = false; // the end timer leads to the next mission instead of the menu
	Text introText;
	bool paused = false;
	int lastHp = -1; // to notice when the player gets hit
	bool pauseHeld = false; // so holding P doesn't flip pause on and off every frame
	bool muteHeld = false;
public:
	// the score is carried over from the mission before
	PlayState(GameStateManager& gsm, int mission = 1, int startScore = 0)
		: gsManager(gsm), mission(mission), world(mission), player(2 * World::CELL, world.surfaceY(2) - 96), score(startScore) {
		entityManager.setPlayer(&player);
		entityManager.setScore(&score);
	}
	void handleInput() override;
	void enter()           override;
	void exit()            override;
	void update(float dt)  override;
	void render(RenderWindow& w) override;
	void drawGoalFlag(RenderWindow& w);
};
