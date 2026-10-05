#pragma once
#include "Soldier.h"
#include "TransformativeState.h"
#include "ProjectileWeapon.h"
#include "WeaponsEach.h"
using namespace sf;
class PlayerSoldier: public Soldier
{
private:
	int characterType;
	float stateTimer;
	bool powerUPActive;
	float powerUPTimer;
	float powerUPCooldown = 0; // seconds until Q can be used again
	bool isDead;
	float drawScale; // how much the picture is enlarged when drawn

	float fireRateMultiplier;
	float speedMultiplier;
	float damageMultiplier;
	int bonusAmmoPercent; //for fio
public:
	PlayerSoldier(float x, float y, int characterType) :Soldier(x, y, 64.f, 96.f, 100), // w, h, hp
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
		currLives = 1;
		totalLives = 1;
		grenadeCount = 10;
		inventory[0] = new Pistol();


		//each character stats

		if (characterType == 0) {//marco
			fireRateMultiplier = 1.25;
			grenadeCount = 8;

			texture.loadFromFile("25I-0504_25I-0644_Assets/marco.png");
			sprite.setTexture(texture);

		}
		else if (characterType == 1) {//tarma
			speedMultiplier = 0.8;

			texture.loadFromFile("25I-0504_25I-0644_Assets/tarma.png");
			sprite.setTexture(texture);

		}
		else if (characterType == 2) { //eri
			grenadeCount = 20;
			fireRateMultiplier = 0.8;
			texture.loadFromFile("25I-0504_25I-0644_Assets/eri.png");
			sprite.setTexture(texture);

		}
		else if (characterType == 3) {//fio
			fireRateMultiplier = 1.1;
			bonusAmmoPercent = 50;
			grenadeCount = 8;

			texture.loadFromFile("25I-0504_25I-0644_Assets/fio.png");
			sprite.setTexture(texture);

		}

		// marco and eri are original size pictures, tarma and fio were already enlarged 4x
		drawScale = (characterType == 1 || characterType == 3) ? PIXEL_SCALE / 4 : PIXEL_SCALE;

	}
	~PlayerSoldier() {
		delete currState;
	}

	void activePowerUp() override;
	void die() override;
	void takeDamage(int amount) override;   // Fio's shield blocks damage
	void spreadShots(Projectile* extra[2]); // Tarma's power up: two more bullets at +-15 degrees
	const char* getPowerUpName() const;
	float getPowerUpCooldown() const { return powerUPCooldown; }
	const char* getName() const override;
	

	void infect(int type); //0 for undead and 1 for mummy

	void update(float frameTime, const World& w) override;
	void render(RenderWindow& window, const Camera& cam) override;

	// character specific
	int  getCharacterType() const { return characterType; }
	bool getIsDead()        const { return isDead; }
	bool getPowerUpActive() const { return powerUPActive; }
	int  getGrenadeCount()  const { return grenadeCount; }
	float getFireRateMultiplier() const { return fireRateMultiplier; }
	float getSpeedMultiplier()    const { return speedMultiplier; }

	void setPiloting(bool p) { piloting = p; }

	bool canShoot() const { return currState->canUseWeapon(); }
	bool canUseMelee() const { if (characterType == 2) {
		return false;
	} return currState->canUseMelee(); }
	Projectile* fire();


	//movement
	void moveLeft(float frameTime) { accelerate(-walkSpeed * 10 * frameTime); };
	void moveRight(float frameTime) { accelerate(walkSpeed * 10 * frameTime); };
	void setSpeedMultiplier(float xAmmount) { speedMultiplier = xAmmount; }
	void jump() { Soldier::jump(); };
	void crouch(bool c) { Soldier::crouch(c); };


	Projectile* throwGrenade() {
		bool unlimited = powerUPActive && characterType == 2; // Eri's power up: grenades don't run out
		if (grenadeCount <= 0 && !unlimited) return nullptr;
		if (!unlimited) grenadeCount -= 1;
		float spawnX = facingRight ? (x + width) : x;
		float spawnY = y + height / 3;

		float dx, dy;
		ProjectileWeapon::getDirection(aimAngle, facingRight, dx, dy);
		return new NormalGrenade(spawnX, spawnY, dx, dy, true);
	}

	void meleeAttack() {
		if (!canUseMelee()) {
			return;
		}
		meleeActive = true;
	}
	
};

