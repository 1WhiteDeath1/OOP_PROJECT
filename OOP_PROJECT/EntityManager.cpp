#include "EntityManager.h"
#include "Soldier.h"
EntityManager::EntityManager() {
	for (int i = 0; i < MAX_PROJECTILES; i++) 
		projectiles[i] = nullptr;
	for (int i = 0; i < MAX_ENEMIES; i++)
		enemies[i] = nullptr;
	for (int i = 0; i < MAX_SOLDIERS; i++)
		soldiers[i] = nullptr;

}

Soldier* EntityManager::getSoldierCurr(int index) const {
	if (index<0|| index >= MAX_SOLDIERS) return nullptr;
	return  soldiers[index];
}


void EntityManager::addSoldier(Soldier* c) {
	if (sCount >= MAX_SOLDIERS) return;
	for (int i = 0; i < MAX_SOLDIERS;i++) {
		if (!soldiers[i]) {
			soldiers[i] = c;
			sCount += 1;
			return;
		}
	}
}

void EntityManager::update(float frameTime, const World& w) {
	
	//updating soldiers
	for (int i = 0; i < MAX_SOLDIERS;i++) {
		if (soldiers[i] && soldiers[i]->getActive()) {
			soldiers[i]->update(frameTime, w);
		}
	}

	//updating enemies
	for (int i = 0; i < MAX_ENEMIES;i++) {
		if (!enemies[i]) continue;
		if (enemies[i]->getActive()) {
			enemies[i]->update(frameTime, w);
		}
	}

	//projectiles
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i]) continue;

		if (projectiles[i]->getActive()) {
			projectiles[i]->update(frameTime, w);
		}
	}

	checkGrenadeBlast();

	//updating enemies
	for (int i = 0; i < MAX_ENEMIES;i++) {
		if (enemies[i] && !enemies[i]->getActive()) {
			delete enemies[i];
			enemies[i] = nullptr;
			eCount -= 1;
		}
	}

	//projectiles
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (projectiles[i] && !projectiles[i]->getActive()) {
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

	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i] || !projectiles[i]->getActive() || !projectiles[i]->isFromPlayer()) continue;

		for (int j = 0; j < MAX_ENEMIES;j++) {
			if (!enemies[j] || !enemies[j]->getActive()) {
				continue;
			}

			if (projectiles[i]->collision(*enemies[j])) {
				enemies[j]->onHitByProjectile(projectiles[i]);
				if (!projectiles[i]->getActive()) break;
			}

		}


	}
}

void EntityManager::checkGrenadeBlast() {
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i] || !projectiles[i]->didExplode() || projectiles[i]->getBlastRadius() <= 0) {
			continue;
		}

		//blast locaiton
		float bx = projectiles[i]->getX() + projectiles[i]->getWidth() / 2.f;
		float by = projectiles[i]->getY() + projectiles[i]->getHeight() / 2.f;
		float bradius = projectiles[i]->getBlastRadius();
		int   dmg = projectiles[i]->getDamage();

		for (int j = 0; j < MAX_ENEMIES;j++) {
			if (!enemies[j] || !enemies[j]->getActive() || !enemies[j]->isAlive()) {
				continue;
			}

			//finding the distance from the bomb
			float ex = enemies[j]->getX() + enemies[j]->getWidth() / 2.f;
			float ey = enemies[j]->getY() + enemies[j]->getHeight() / 2.f;

			float diffX = bx - ex;
			float diffY = by - ey;

			float modSquared= (diffX * diffX) + (diffY * diffY);

			if (modSquared < (bradius * bradius)) {
				enemies[j]->onHitByProjectiles(projectiles[i]);
			}

		}

		//check for the soldiers too
		for (int j = 0; j < MAX_SOLDIERS;j++) {
			if (!soldiers[j] || !soldiers[j]->getActive() || !soldiers[j]->isAlive()) {
				continue;
			}

			//finding the distance from the bomb
			float sx = soldiers[j]->getX() + soldiers[j]->getWidth() / 2.f;
			float sy = soldiers [j] ->getY() + soldiers[j]->getHeight() / 2.f;

			float diffX = bx - sx;
			float diffY = by - sy;

			float modSquared = (diffX * diffX) + (diffY * diffY);

			if (modSquared < (bradius * bradius)) {
				soldiers[j]->takeDamage(dmg/2);
			}
		}
	}
}

void EntityManager::checkEnemyPlayerCollisions(Player& player) {
	PlayerSoldier* active = player.getActive();

	for (int i = 0; i < eCount;i++) {
		if (!enemies[i]0 > getActive()) continue;

		if (active->collision(*enemies[i])) {
			int type = enemies[i]->getEnemyType;

			if (type == 1) {
				active->infect(1);
			}
			else if (type == 2) {
				active->infect(2);
			}
			else {
				active->takeDamage(enemies[i]->getDamage());
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