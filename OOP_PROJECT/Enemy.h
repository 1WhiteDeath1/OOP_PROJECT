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
	virtual ~EnemyAiState() = default;

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
	RoamingAround(float speed = 50, float range = 450) :roamingAroundSpeed(speed), detectionRange(range), isAlive(true), directionSign(1) {}
	EnemyAiState* update(Enemy* enemy, float dt, const World& w);

};

class AttackingState :public EnemyAiState {
	float attackingRange;
	float attackCoolDown;
	float losingRange;

public:
	AttackingState(float attackingRange = 300, float losingRange = 400) :attackingRange(attackingRange), attackCoolDown(2), losingRange(losingRange) {}
	EnemyAiState* update(Enemy* enemy, float dt, const World& w);
};

class runningState :public EnemyAiState {
	float runningSpeed;
	float attackingRange;
	float losingRange;
public:
	runningState(float runningSpeed = 100, float attackingRange = 300, float losingRange = 550) :runningSpeed(runningSpeed), attackingRange(attackingRange), losingRange(losingRange) {}
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

	// death animation: knocked back into the air, spins and fades, then removed
	bool dying = false;
	bool scored = false; // the kill has been added to the score
	float deathTimer = 0;
	static constexpr float DEATH_TIME = 0.7f;

	// pictures: the standing picture (each soldier type has its own texture) and an optional
	// walking strip of frames side by side. no strip = the standing picture just bounces
	const Texture* standTex = nullptr;
	Texture walkTex;
	int walkFrames = 0;
	void setPictures(const Texture& stand, const char* walkFile, int frames);
	void drawEnemy(RenderWindow& w, const Camera& cam, bool pictureFacesLeft);
public:
	void aimAtTarget(float& dx, float& dy) const; // unit direction from the enemy's gun to the target
protected:
public:
	Soldier* target;
	float drawScale = PIXEL_SCALE; // how much the picture is enlarged when drawn
	// returns the points for this kill the first time it is called after the enemy died, 0 otherwise
	int takeKillPoints() {
		if (isAlive() || scored) return 0;
		scored = true;
		if (typeOfEnemy == 4) return 5000;     // the boss
		return typeOfEnemy == 1 ? 200 : 100;   // shield soldier is worth more
	}
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
	void die() override;   // starts the death animation instead of disappearing at once
	virtual bool isBoss() const { return false; }
	// lets an enemy ask the EntityManager for an explosion at (ex, ey); used by the boss while it blows up
	virtual bool takeExplosion(float& ex, float& ey) { return false; }
	void onHitByProjectile(Projectile* p);


	int getDamage() {
		return damageDeals;
	}
	virtual void TakeNormalDamage(Projectile* p) {
		p->applyDamage(this);
	}
	virtual void takeExplosionDamage(Projectile* p) {
		p->applyDamage(this);
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
		: Enemy(x, y, wd, ht, hp, 0, 10, 1, new RoamingAround()) {
		texture.loadFromFile("25I-0504_25I-0644_Assets/rebel.png");
		sprite.setTexture(texture);
		setPictures(texture, "25I-0504_25I-0644_Assets/bazooka_walk.png", 11);
		drawScale = PIXEL_SCALE / 4; // this picture was already enlarged 4x
	}
	void throwProjectile() override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
};

class ShieldedSoldier :public Enemy {
	sf::Texture texture;
public:
	ShieldedSoldier(float x, float y, float wd, float ht, float hp)
		: Enemy(x, y, wd, ht, hp, 1, 10, 1, new RoamingAround()) { // type 1 = shield soldier
		texture.loadFromFile("25I-0504_25I-0644_Assets/shielded.png");
		sprite.setTexture(texture);
		setPictures(texture, nullptr, 0);
		drawScale = 100.f / texture.getSize().y; // not a Metal Slug picture, so just make it soldier height
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
		: Enemy(x, y, wd, ht, hp, 0, 10, 3, new RoamingAround()) {
		texture.loadFromFile("25I-0504_25I-0644_Assets/bazooka.png");
		sprite.setTexture(texture);
		setPictures(texture, "25I-0504_25I-0644_Assets/bazooka_walk.png", 11);
	}
	void throwProjectile() override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
};

class GrenadeSoldier :public Enemy {
	sf::Texture texture;
public:
	GrenadeSoldier(float x, float y, float wd, float ht, float hp)
		: Enemy(x, y, wd, ht, hp, 3, 10, 2.5f, new RoamingAround()) { // type 3 = grenade soldier
		texture.loadFromFile("25I-0504_25I-0644_Assets/grenade_soldier.png");
		sprite.setTexture(texture);
		setPictures(texture, "25I-0504_25I-0644_Assets/rebel_run.png", 12);
	}
	void throwProjectile() override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
};