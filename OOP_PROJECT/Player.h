#pragma once
#include "PlayerSoldier.h"
#include "Vehicle.h"
class EntityManager;
class World;

class Player
{
private:
	PlayerSoldier* characters[4];
	int activeIndex;
	Vehicle* vehicle;

	// keys that should only act once per press, not every frame they are held
	bool switchHeld = false, grenadeHeld = false, meleeHeld = false, powerHeld = false;

public:
	Player(float spawnX, float spawnY) : activeIndex(0), vehicle(nullptr) {
		characters[0] = new PlayerSoldier(spawnX, spawnY, 0); // Marco
		characters[1] = new PlayerSoldier(spawnX, spawnY, 1); // Tarma
		characters[2] = new PlayerSoldier(spawnX, spawnY, 2); // Eri
		characters[3] = new PlayerSoldier(spawnX, spawnY, 3); // Fio
	}
	~Player() {
		for (int i = 0; i < 4; i++)
		{
			delete characters[i];
			characters[i] = nullptr;
		}
		// the vehicle belongs to the EntityManager, it deletes it
	}

	void handleInput(float frameTime, const World& w, EntityManager& eManager);
	void update(float frameTime, const World& w);
	void render(RenderWindow& windowm, const Camera& cam);

	void switchCharacter();
	void mountVehicle(Vehicle* v);
	void dismountVehicle();

	PlayerSoldier* getActive() const { return characters[activeIndex]; }
	PlayerSoldier* getSoldier(int i) const { return characters[i]; }
	Vehicle* getVehicle() const { return vehicle; }
	bool isPiloting() const { return vehicle != nullptr; }
	bool allDead() const;
};

