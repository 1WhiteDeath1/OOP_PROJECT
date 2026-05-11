#pragma once
#include "DamagableEntity.h"
#include "Soldier.h"
class World;
class Enemy;

class EnemyAiState {

public:
	virtual void enterState(Enemy* enemy) {}
	virtual EnemyAiState* update(Enemy* enemy, float dt, const World& w) = 0;
	virtual void exit() {}

};

class RoamingAround;
class AttackingState;
class runningState;


class RoamingAround :public EnemyAiState {
protected:

	float roamingAroundSpeed;
	float detectionRange;
	bool isAlive;
	float timerToSwitchDirection = 4;
	int directionSign;
public:
	RoamingAround(float speed = 50, float range = 250) :roamingAroundSpeed(speed), detectionRange(range), isAlive(true), directionSign(1) {}
	EnemyAiState* update(Enemy* enemy, float dt, const World& w);

};

class AttackingState :public EnemyAiState {
	float attackingRange;
	float attackCoolDown;
	float losingRange;

public:
	AttackingState(float attackingRange = 50, float losingRange = 100) :attackingRange(attackingRange), attackCoolDown(2), losingRange(losingRange) {}
	EnemyAiState* update(Enemy* enemy, float dt, const World& w);
};

class runningState :public EnemyAiState {
	float runningSpeed;
	float attackingRange;
	float losingRange;
public:
	runningState(float runningSpeed = 100, float attackingRange = 50, float losingRange = 200) :runningSpeed(runningSpeed), attackingRange(attackingRange), losingRange(losingRange) {}
	EnemyAiState* update(Enemy* enemy, float dt, const World& w)override;


};
class Descending :public EnemyAiState {
	float descendingSpeed;
public:
	EnemyAiState* update(Enemy* enemy, float dt, const World& w)override;
};

class Enemy :public DamagableEntity {
protected:
	int typeOfEnemy;
	int damageDeals;
	float normalFireRate;
	EnemyAiState* currentAiState;
	Projectile* projectile;
public:
	Soldier* target;
	Enemy(float x, float y, float wd, float ht, int hp,
		int typeOfEnemy, int damageDeals, float normalFireRate,
		EnemyAiState* currentState)
		: DamagableEntity(x, y, wd, ht, hp), typeOfEnemy(typeOfEnemy),
		damageDeals(damageDeals), normalFireRate(normalFireRate),
		target(nullptr), currentAiState(currentState), projectile(nullptr) {
	}
	~Enemy() {
		delete currentAiState;
		delete projectile;
	}
	void update(float dt, const World& w) override;
	void onHitByProjectile(Projectile* p);


	int getDamage() {
		return damageDeals;
	}
	virtual void TakeNormalDamage(Projectile* p) {
		takeDamage(p->getDamage());
	}
	virtual void takeExplosionDamage(Projectile* p) {
		takeDamage(p->getDamage());
	}
	void setVelocityX(float v) {
		velocityX = v;
	}
	void setVelocityY(float v) {
		velocityY = v;
	}
	float getNormalFireRate()const {
		return normalFireRate;
	}
	virtual void throwProjectile() = 0;
	void changeXandY(float dt, const World& w);
	Projectile* getProjectile();
};










class RebelSoldier :public Enemy {
	sf::Texture texture;
public:
	RebelSoldier(float x, float y, float wd, float ht, float hp)
		: Enemy(x, y, wd, ht, hp, 0, 5, 1, new RoamingAround()) {
		texture.loadFromFile("i250504_i250644_Assets/rebel.png");
		sprite.setTexture(texture);
		sprite.setScale(64.f / 155, 96.f / 194);
	}
	void throwProjectile() override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
};

class ShieldedSoldier :public Enemy {
	sf::Texture texture;
public:
	ShieldedSoldier(float x, float y, float wd, float ht, float hp)
		: Enemy(x, y, wd, ht, hp, 0, 5, 1, new RoamingAround()) {
		texture.loadFromFile("i250504_i250644_Assets/shielded.png");
		sprite.setTexture(texture);
		sprite.setScale(64.f / 1088, 96.f / 1190);
	}
	void throwProjectile() override;
	void TakeNormalDamage(Projectile* p)override;
	void takeExplosionDamage(Projectile* p)override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
};


class BazookaSoldier :public Enemy {
	sf::Texture texture;
public:
	BazookaSoldier(float x, float y, float wd, float ht, float hp)
		: Enemy(x, y, wd, ht, hp, 0, 5, 3, new RoamingAround()) {
		texture.loadFromFile("i250504_i250644_Assets/bazooka.png");
		sprite.setTexture(texture);
		sprite.setScale(64.f / 48, 96.f / 43);
	}
	void throwProjectile() override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
};

class GrenadeSoldier :public Enemy {
	sf::Texture texture;
public:
	GrenadeSoldier(float x, float y, float wd, float ht, float hp)
		: Enemy(x, y, wd, ht, hp, 0, 5, 2.5f, new RoamingAround()) {
		texture.loadFromFile("i250504_i250644_Assets/grenade_soldier.png");
		sprite.setTexture(texture);
		sprite.setScale(64.f / 36, 96.f / 50);
	}
	void throwProjectile() override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
};