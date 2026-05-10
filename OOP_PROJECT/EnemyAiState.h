#pragma once
#include "DamagableEntity.h"
#include "Soldier.h"
class World;
class Enemy;

class EnemyAiState {

	public:
	virtual void enterState(Enemy* enemy){}
	virtual EnemyAiState* update(Enemy* enemy, float dt, const World& w)=0;
	virtual void exit(){}

};

class RoamingAround;
class AttackingState;
class runningState;


class RoamingAround:public EnemyAiState {
protected:

	float roamingAroundSpeed;
	float detectionRange;
	bool isAlive;
	float timerToSwitchDirection=4;
	int directionSign;
	public:
	RoamingAround(float speed=50, float range=250):roamingAroundSpeed(speed),detectionRange(range),isAlive(true),directionSign(1){}


	EnemyAiState* update(Enemy* enemy, float dt, const World& w) {
	if(enemy->target==nullptr)
	return nullptr;
	float coordinateX=enemy->getX();
	float coordinateY=enemy->getY();
	float playerX=enemy->target->getX();
	float playerY=enemy->target->getY();

	if(((coordinateX-playerX<0?playerX-coordinateX:coordinateX-playerX)<detectionRange)&&(coordinateY-playerY<0?playerY-coordinateY:coordinateY-playerY)<detectionRange)
	return new runningState();

	timerToSwitchDirection-=dt;
	if (timerToSwitchDirection <= 0)
	{
	directionSign*=-1;
	timerToSwitchDirection=4;
	}

	enemy->setVelocityX(directionSign*roamingAroundSpeed);
	enemy->applyGravity(dt);
	enemy->changeXandY(dt,w);
	
	

	
	return nullptr;
	}

};

class AttackingState :public EnemyAiState {
	float attackingRange;
	float attackCoolDown;
	float losingRange;

public:
	AttackingState(float attackingRange = 50, float losingRange = 100) :attackingRange(attackingRange), attackCoolDown(2), losingRange(losingRange) {}

	EnemyAiState* update(Enemy* enemy, float dt, const World& w) {
		if (enemy->target == nullptr)
			return nullptr;
		float coordinateX = enemy->getX();
		float coordinateY = enemy->getY();
		float playerX = enemy->target->getX();
		float playerY = enemy->target->getY();
		bool right = playerX > coordinateX;
		if (((coordinateX - playerX < 0 ? playerX - coordinateX : coordinateX - playerX) > losingRange) && (coordinateY - playerY < 0 ? playerY - coordinateY : coordinateY - playerY) > losingRange)
			return new runningState();

		attackCoolDown -= dt;
		if (attackCoolDown <= 0) {
			attackCoolDown = enemy->getNormalFireRate();
			enemy->throwProjectile();
		}

		enemy->setVelocityX(0);
		enemy->applyGravity(dt);
		enemy->changeXandY(dt, w);
		return nullptr;
	}


};

class runningState :public EnemyAiState {
float runningSpeed;
float attackingRange;
float losingRange;
public:
	runningState(float runningSpeed = 100, float attackingRange = 50,float losingRange=200):runningSpeed(runningSpeed),attackingRange(attackingRange),losingRange(losingRange) {}
	EnemyAiState* update(Enemy* enemy, float dt, const World& w)override {
		if(enemy->target==nullptr)
		return nullptr;

		float coordinateX=enemy->getX();
	float coordinateY=enemy->getY();
	float playerX=enemy->target->getX();
	float playerY=enemy->target->getY();
	bool right = playerX > coordinateX;

	if(((coordinateX-playerX<0?playerX-coordinateX:coordinateX-playerX)<attackingRange)&&(coordinateY-playerY<0?playerY-coordinateY:coordinateY-playerY)<attackingRange)
	return new AttackingState();
	else 
	if(((coordinateX-playerX<0?playerX-coordinateX:coordinateX-playerX)>losingRange)&&(coordinateY-playerY<0?playerY-coordinateY:coordinateY-playerY)>losingRange)
	return new RoamingAround();

	enemy->setVelocityX(right?runningSpeed:-runningSpeed);
	enemy->applyGravity(dt);
	enemy->changeXandY(dt,w);
	return nullptr;
	}
	
	
};

class Descending :public EnemyAiState {
	float descendingSpeed;
	public:
		EnemyAiState* update(Enemy* enemy, float dt, const World& w)override {
			enemy->setVelocityX(0);
			enemy->setVelocityY(descendingSpeed);
			enemy->changeXandY(dt,w);
			if(enemy->isTouchingGround(w))
			return new RoamingAround();
			return nullptr;
	}


};