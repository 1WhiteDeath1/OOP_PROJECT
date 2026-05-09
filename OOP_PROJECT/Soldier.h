#pragma once
#include "DamagableEntity.h"
#include "TransformativeState.h"
#include "Weapon.h"
class Soldier: public DamagableEntity
{
protected:
	TransformativeState* currState = nullptr;
	Weapon* inventory[2] = { nullptr, nullptr };

	int currWeapon = 0;
	int grenadeCount = 3;
	int currLives = 3;
	int totalLives = 3;

	bool isJumping = false;
	bool isCrouching = false;
	bool facingRight = true;
	bool inWater = false;
	bool piloting = false;

	float aimAngle = 0;
	float walkSpeed = 300;
	float jumpStrength = 700;

	bool meleeActive = false;
public:
	Soldier(float x, float y, float w, float h, int hp);
	virtual ~Soldier();

	virtual void activePowerUp() = 0;
	virtual const char* getName() const = 0;

	void jump();
	void crouch(bool c);
	void movement(float frameTime, const World& w);
	void changeAngle(float angle);
	void respawn(float spawnX, float spawnY);
	void setWeapon(int slot, Weapon* wp);

	void accelerate(float amount) {
		velocityX += amount;
	}

	void setFacing(bool right) {
		facingRight = right;
	}

	Projectile* fireWeapon();

	//getters all
	Weapon* getActiveWeapon() const {
		return inventory[currWeapon];
	}
	int getLives() const {
		return currLives;
	}
	int getTotalLives() const {
		return totalLives;
	}
	bool isFacingRight() const {
		return facingRight;
	}
	float getAimAngle() const {
		return aimAngle;
	}
	bool getIsJumping() const {
		return isJumping;
	}
	bool getIsCouching() const {
		return isCrouching;
	}
	bool isMeleeActive() const {
		return meleeActive; }



	bool checkMeleeCollision(const Entity& target) const {


		float Mx = facingRight ? (x + width) : (x - 50.f);
		float mMidX = Mx + 25.f;
		float mMidY = y + height / 2.f;
		float tMidX = target.getX() + target.getWidth() / 2.f;
		float tMidY = target.getY() + target.getHeight() / 2.f;

		bool hitX = abs(mMidX - tMidX) < (25.f + target.getWidth() / 2.f);
		bool hitY = abs(mMidY - tMidY) < (height / 2.f + target.getHeight() / 2.f);
		return hitX && hitY;


	}
	
};

