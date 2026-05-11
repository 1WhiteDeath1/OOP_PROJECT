#pragma once
#include "Vehicle.h"

class AquaticVehicle :public Vehicle {
public:

	AquaticVehicle(float x, float y, float wd, float ht, int hp, int fr, int vd) :Vehicle(x, y, wd, ht, hp, fr, vd) {
		texture.loadFromFile("i250504_i250644_Assets/aquaticVehicle.png");
		sprite.setTexture(texture);
	}
	virtual ~AquaticVehicle(){}
};



class SlugMariner :public AquaticVehicle {
int maxMissileAmmo;
int horizontalMissileAmmo;
int verticalMissileAmmo;
int reverseProjectileAmmo;

public:
SlugMariner(float x, float y, float wd, float ht, int hp, int fr, int vd) :AquaticVehicle(x, y, wd, ht, hp, fr, vd) {
maxMissileAmmo=horizontalMissileAmmo=verticalMissileAmmo=reverseProjectileAmmo=3;
texture.loadFromFile("i250504_i250644_Assets/slugMariner.png");
sprite.setTexture(texture);
}
	void attack() override;
	void move(float dt, const World& w) override;
	void update(float dt, const World& w) override;
	void render(sf::RenderWindow& w, const Camera& cam) override;
};

class EnemySub :public AquaticVehicle {
Soldier * playerToHit=nullptr;
public:
	EnemySub(float x, float y, float wd, float ht, int fr, int vd) :AquaticVehicle(x, y, wd, ht, 7, fr, vd) {
		texture.loadFromFile("i250504_i250644_Assets/enemySub.png");
		sprite.setTexture(texture);
	}

	void attack() override;
	void move(float dt, const World& w) override;
	void update(float dt, const World& w) override;
	void render(sf::RenderWindow& w, const Camera& cam) override;
	void setPlayerToHit(Soldier* s){playerToHit=s;}

};