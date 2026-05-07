#include "EntityManager.h"

EntityManager::EntityManager() {
	for (int i = 0; i < MAX_PROJECTILES; i++) 
		projectiles[i] = nullptr;
	for (int i = 0; i < MAX_ENEMIES; i++)
		enemies[i] = nullptr;
	for (int i = 0; i < MAX_SOLDIERS; i++)
		soldiers[i] = nullptr;

}

Soldier* EntityManager::getSoldier(int index) const {
	if (index<0|| index >= MAX_SOLDIERS) return nullptr;
	return  soldiers[index];
}


void EntityManager::addSoldier(Soldier* c) {
	if (soldierCount >= MAX_SOLDIERS) return;
	for (int i = 0; i < MAX_SOLDIERS;i++) {
		if (!soldiers[i]) {
			soldiers[i] = c;
			soldierCount += 1;
			return;
		}
	}
}

void EntityManager::update(float frameTime) {
	
	//updating soldiers
	for (int i = 0; i < MAX_SOLDIERS;i++) {
		if (soldiers[i] && soldiers[i]->getActive()) {
			soldiers[i]->update(frameTime);
		}
	}

	//updating enemies
	for (int i = 0; i < MAX_ENEMIES;i++) {
		if (!enemies[i]) continue;
		if (enemies[i]->getActive()) {
			enemies[i]->update(frameTime);
		}
		if (!enemies[i]->getActive()) {
			delete enemies[i];
				enemies[i] = nullptr;
			eCount -= 1;
		}
	}

	//projectiles
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i]) continue;

		if (projectiles[i]->getActive()) {
			projectiles[i]->update(frameTime);
		}

		if (!projectiles[i]->getActive()) {
			delete projectiles[i];
			projectiles[i] = nullptr;
			pCount -= 1;
		}
	}

	if (coolDown > 0) {
		coolDown -= frameTime;
	}
}

void EntityManager::render(RenderWindow& w, Camera& cam) {
	for (int i = 0; i < MAX_ENEMIES; i++)
		if (enemies[i] && enemies[i]->getActive())
			enemies[i]->render(w, cam);

	for (int i = 0; i < MAX_SOLDIERS; i++)
		if (soldiers[i] && soldiers[i]->getActive())
			soldiers[i]->render(w, cam);

	for (int i = 0; i < MAX_PROJECTILES; i++)
		if (projectiles[i] && projectiles[i]->getActive())
			projectiles[i]->render(w, cam);
}

void EntityManager::checkProjectileWorldCollisions(World& w) {
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i] || !projectiles[i]->getActive()) {
			continue;
		}

		float midX = projectiles[i]->getX() + projectiles[i]->getWidth() / 2.f;
		float midY = projectiles[i]->getY() + projectiles[i]->getHeight() / 2.f;

		if (w.isSolid(midX, midY))
			projectiles[i]->setActive(false);
	}
}

void EntityManager::checkProjectileCollisions() {
	if (coolDown > 0) return;

	for (int i = 0; i < MAX_SOLDIERS;i++) {
		if (!soldiers[i] || !soldiers[i]->getActive() || !soldiers[i]->isAlive()) continue;

		for (int j = 0; j < MAX_ENEMIES;j++) {
			if (!enemies[j] || !enemies[j]->getActive()) {
				continue;
			}

		}


	}
}

EntityManager::~EntityManager() {
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		if (projectiles[i]) { delete projectiles[i]; projectiles[i] = nullptr; }
	}
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (enemies[i]) { delete enemies[i]; enemies[i] = nullptr; }
	}
	// Soldiers are owned — delete them too
	for (int i = 0; i < MAX_SOLDIERS; i++)
	{
		if (soldiers[i]) { delete soldiers[i]; soldiers[i] = nullptr; }
	}
	pCount = eCount = sCount = 0;
}