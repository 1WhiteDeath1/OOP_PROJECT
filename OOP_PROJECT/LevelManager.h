#pragma once
#include "Level.h"


class LevelManager
{
private:
	int currLevel = 1;
	int MAX = 3; // missions 1, 2 and 3 (the boss)
	Level* level = nullptr;
public:
	LevelManager() {}
	~LevelManager() { delete level; }

	void loadLevel(int n, const World& w, EntityManager& em) {
		delete level;
		currLevel = n;
		level = new Level(n, w);
		level->setUP(em);
	}

	// missions with a goal flag end when the player reaches it, the boss mission when the boss is destroyed
	bool isLevelFinished(const EntityManager& em, float playerX) const {
		if (level && level->getGoalColumn() > 0)
			return playerX >= level->getGoalColumn() * World::CELL;
		return em.isBossDefeated();
	}
	int getGoalColumn() const { return level ? level->getGoalColumn() : 0; }
	void update(float dt, float playerX, const World& w, EntityManager& em) {
		if (level) level->checkWaves(dt, playerX, w, em);
	}
	int getCurrLevel() const {
		return currLevel;
	}
	bool nextLevelAvalaible() const {
		if (currLevel < MAX) {
			return true;
		}
		return false;
	}
};

