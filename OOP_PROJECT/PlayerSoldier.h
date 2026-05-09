#pragma once
#include "Soldier.h"
#include "TransformativeState.h"
using namespace sf;
class PlayerSoldier: public Soldier
{
private:
	int characterType;
	float stateTimer;
	bool powerUPActive;
	float powerUPTimer;
	bool isDead;

	float fireRateMultiplier;
	float speedMultiplier;
	float damageMultiplier;
	int bonusAmmoPercent; //for fio
public:
	PlayerSoldier(float x, float y, int characterType) :Soldier(x, y, 32, 48, 100), // w, h, hp
		characterType(characterType),
		stateTimer(0),
		powerUPActive(false),
		powerUPTimer(0),
		isDead(false),
		fireRateMultiplier(1.0f),
		speedMultiplier(1.0f),
		damageMultiplier(1.0f),
		bonusAmmoPercent(0) {


		currState = new NormalState();
		currLives = 2;
		totalLives = 2;
		grenadeCount = 10;

		//each character stats

		if (characterType == 0) {//marco
			fireRateMultiplier = 1.25;
			grenadeCount = 8;
		}
		else if (characterType == 1) {//tarma
			speedMultiplier = 0.8;
		}
		else if (characterType == 2) { //eri
			grenadeCount = 20;
			fireRateMultiplier = 0.8;
		}
		else if (characterType == 3) {//fio
			fireRateMultiplier = 1.1;
			bonusAmmoPercent = 50;
			grenadeCount = 8;
		}

	}
	~PlayerSoldier() {
		delete currState;
	}

	void activePowerUp() override;
	const char* getName() const override;
	

	void infect(int type); //0 for undead and 1 for mummy

	void update(float frameTime, const World& w) override;
	void render(RenderWindow& window, Camera& cam) override;

	// character specific
	int  getCharacterType() const { return characterType; }
	bool getIsDead()        const { return isDead; }
	bool getPowerUpActive() const { return powerUPActive; }
	float getFireRateMultiplier() const { return fireRateMultiplier; }
	float getSpeedMultiplier()    const { return speedMultiplier; }

	void setPiloting(bool p) { piloting = p; }

	bool canShoot() const { return currState->canUseWeapon(); }
	bool canUseMelee() const { if (characterType == 2) {
		return false;
	} return currState->canUseMelee(); }

	Projectile* fire();


	//movement
	void moveLeft(float frameTime) { accelerate(-walkSpeed * 3 * frameTime); };
	void moveRight(float frameTime) { accelerate(walkSpeed * 3 * frameTime); };
	void jump() { Soldier::jump(); };
	void crouch(bool c) { Soldier:crouch(c); };
	void throwGrenade();
	void meleeAttack();
};

