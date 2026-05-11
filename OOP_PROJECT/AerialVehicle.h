#pragma once
#include "Vehicle.h"
#include "GroundVehicle.h"

class AerialVehicle :public Vehicle {
	float hoveringY;
public:
	AerialVehicle(float x, float y, float wd, float ht, int hp, int fr, int vd,float Y) :Vehicle(x, y, wd, ht, hp, fr, vd) {
	hoveringY=Y;
	}
	virtual ~AerialVehicle(){}

};
class FlyingTara :public AerialVehicle {
	float timeBeforeNextGrenade;
	float currentTime;
	Soldier* playerToHit;

public:
	FlyingTara(float x, float y, float wd, float ht, int hp, int fr, int vd,float Y) :AerialVehicle(x, y, wd, ht, hp, fr, vd,Y), playerToHit(nullptr) {
		texture.loadFromFile("25I-0504_25I-0644_Assets/flyingTara.png");
		sprite.setTexture(texture);
	}
	void attack() override;
	void move(float dt, const World& w)override;
	void update(float dt, const World& w)override;
	void render(sf::RenderWindow& w, const Camera& cam)override;
	void playerInSight(Soldier* s);

};
class SlugFlyer :public AerialVehicle {
int missileCount=4;
float missileCountDown=1;
public:
	SlugFlyer(float x, float y, float wd, float ht, int hp, int vd,float Y) :AerialVehicle(x, y, wd, ht, hp, (MetalSlug::metalSlugFireRate) / 2, vd,Y) {
		texture.loadFromFile("25I-0504_25I-0644_Assets/slugFlyer.png");
		sprite.setTexture(texture);
	}
	void attack() override;
	void move(float dt, const World& w)override;
	void update(float dt, const World& w)override;
	void render(sf::RenderWindow& w, const Camera& cam)override;


};