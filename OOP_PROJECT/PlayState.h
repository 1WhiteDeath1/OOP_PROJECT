#pragma once
#include "GameState.h"
#include "GameStateManager.h"
#include "World.h"
#include "Player.h"
#include "EntityManager.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class PlayState : public GameState {
	GameStateManager& gsManager;
	World world;
	Player player;
	EntityManager entityManager;
	Camera camera;
public:
	PlayState(GameStateManager& gsm) : gsManager(gsm), player(100, 600) {
		entityManager.setPlayer(&player);
	}
	void enter()           override {}
	void exit()            override {}
	void update(float dt)  override;
	void render(RenderWindow& w) override;
};
