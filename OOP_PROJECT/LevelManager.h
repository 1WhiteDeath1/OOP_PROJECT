#pragma once
#include "Level.h"


class LevelManager
{
private:
	int currLevel = 1;
	int MAX = 1;
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

	bool isLevelFinished(const EntityManager& em) const {
		if (em.getEnemyCount() == 0) {
			return true;
	}
		return false;
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

