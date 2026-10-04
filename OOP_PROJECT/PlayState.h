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
#include <SFML/Graphics.hpp>
using namespace sf;

class PlayState : public GameState {
	GameStateManager& gsManager;
	World world;
	Player player;
	EntityManager entityManager;
	LevelManager levelManager;
	Camera camera;
	DayNightCycle dayNight;
	WeatherSystem weather;

	Font font;
	Text hudText;
	Text bannerText;
	float endTimer = 0; // counts down after game over / mission complete before going back to the menu
	bool paused = false;
	bool pauseHeld = false; // so holding P doesn't flip pause on and off every frame
public:
	PlayState(GameStateManager& gsm) : gsManager(gsm), player(2 * World::CELL, world.surfaceY(2) - 96) {
		entityManager.setPlayer(&player);
	}
	void handleInput() override;
	void enter()           override;
	void exit()            override {}
	void update(float dt)  override;
	void render(RenderWindow& w) override;
};
