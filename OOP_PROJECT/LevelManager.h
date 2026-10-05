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

	// missions with a goal flag end when the player reaches it, the last one when every enemy is gone
	bool isLevelFinished(const EntityManager& em, float playerX) const {
		if (level && level->getGoalColumn() > 0)
			return playerX >= level->getGoalColumn() * World::CELL;
		return em.getEnemyCount() == 0;
	}
	int getGoalColumn() const { return level ? level->getGoalColumn() : 0; }
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

