#pragma once
#include "DamagableEntity.h"
#include "Soldier.h"
#include "EnemyAiState.h"


class EntityManager;

class Enemy :public DamagableEntity {
protected:
	int typeOfEnemy;
	int damageDeals;
	float normalFireRate;

	EnemyAiState* currentAiState;
	Projectile* projectile;


public:
	Soldier* target;
	Enemy(float x, float y, float wd, float ht, int hp, int typeOfEnemy, int damageDeals,
		float normalFireRate, EnemyAiState* currentState) :DamagableEntity(x, y, wd, ht, hp), typeOfEnemy(typeOfEnemy),
		damageDeals(damageDeals), normalFireRate(normalFireRate), target(nullptr), currentAiState(currentState),
		projectile(nullptr) {
	}

	~Enemy() {
		delete currentAiState;
		delete projectile;
	}

	void update(float dt, const World& w)override {
		if (currentAiState == nullptr)
			return;
		EnemyAiState* New = currentAiState->update(this, dt, w);
		if (New != nullptr) {
			delete currentAiState;
			currentAiState = New;
			currentAiState->enterState(this);

		}



	}

	void onHitByProjectile(Projectile* p) {
		p->applyDamage(this);
		if (p->diesOnHit()) p->setActive(false);
	}

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
	void changeXandY(float dt, const World& w) {
		x += velocityX * dt;
		y += velocityY * dt;
		checkGroundCollisions(w);
	}
	Projectile* getProjectile() {
		Projectile* p = projectile;
		projectile = nullptr;
		return p;

	}


};
class RebelSoldier :public Enemy {
	sf::Texture texture;
public:
	RebelSoldier(float x, float y, float wd, float ht, float hp) :Enemy(x, y, wd, ht, hp, 0, 5, 1, new RoamingAround()) {
		texture.loadFromFile("");
		sprite.setTexture(texture);
		sprite.setScale(30, 30);
	}
	void throwProjectile()override {
		bool right = target->getX() > getX();
		projectile = new Bullet(getX(), getY() + 5, right ? 1 : -1, 0, false, 3);

	}
	void render(sf::RenderWindow& w, const Camera& cam)override
	{
		sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
		w.draw(sprite);
	}

};
class ShieldedSoldier :public Enemy {
	sf::Texture texture;
public:
	ShieldedSoldier(float x, float y, float wd, float ht, float hp) :Enemy(x, y, wd, ht, hp, 0, 5, 1, new RoamingAround()) {
		texture.loadFromFile("");
		sprite.setTexture(texture);
		sprite.setScale(30, 30);
	}
	void throwProjectile()override {
		bool right = target->getX() > getX();
		projectile = new Bullet(getX(), getY() + 5, right ? 1 : -1, 0, false, 3);

	}
	void render(sf::RenderWindow& w, const Camera& cam)override
	{
		sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
		w.draw(sprite);
	}
	void TakeNormalDamage(Projectile* p) {
		bool bulletIsRight = p->getX() > getX();
		bool enemyIsRight = (target != nullptr) && (target->getX() > getX());
		if (bulletIsRight != enemyIsRight)
			takeDamage(p->getDamage());
	}
	void takeExplosionDamage(Projectile* p) {
		takeDamage(p->getDamage());

	}

};

class BazookaSoldier :public Enemy {
	sf::Texture texture;
public:
	BazookaSoldier(float x, float y, float wd, float ht, float hp) :Enemy(x, y, wd, ht, hp, 0, 5, 1, new RoamingAround()) {
		texture.loadFromFile("");
		sprite.setTexture(texture);
		sprite.setScale(30, 30);
	}
	void throwProjectile()override {
		bool right = target->getX() > getX();
		projectile = new Rocket(getX(), getY() + 5, right ? 1 : -1, 0, false, 3);

	}
	void render(sf::RenderWindow& w, const Camera& cam)override
	{
		sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
		w.draw(sprite);
	}

};
class GrenadeSoldier :public Enemy {
	sf::Texture texture;
public:
	GrenadeSoldier(float x, float y, float wd, float ht, float hp) :Enemy(x, y, wd, ht, hp, 0, 5, 1, new RoamingAround()) {
		texture.loadFromFile("");
		sprite.setTexture(texture);
		sprite.setScale(30, 30);
	}
	void throwProjectile()override {
		bool right = target->getX() > getX();
		projectile = new NormalGrenade(getX(), getY() + 5, right ? 1 : -1, 0, false, 3);

	}
	void render(sf::RenderWindow& w, const Camera& cam)override
	{
		sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
		w.draw(sprite);
	}

};