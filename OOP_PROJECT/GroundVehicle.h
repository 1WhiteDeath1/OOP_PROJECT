#pragma once
#include "Vehicle.h"

class GroundVehicle :public Vehicle {
public:
	GroundVehicle(float x, float y, float wd, float ht, int hp, int fr, int vd) :Vehicle(x, y, wd, ht, hp, fr, vd) {}
	virtual ~GroundVehicle(){}
};


class M15Bradley :public GroundVehicle {
	float launchingRate;
	float launchingAngle;
	public:
	Soldier* playerToHit;

public:
	M15Bradley(float x, float y, float wd, float ht,int fr, int vd) :GroundVehicle(x, y, wd, ht, 7, fr, vd) {
	playerToHit=nullptr;
	texture.loadFromFile("25I-0504_25I-0644_Assets/M15Bradley.png");
	sprite.setTexture(texture);
	}
	void attack() override;
	void move(float dt, const World& w) override;
	void update(float dt, const World& w)override;
	void render(sf::RenderWindow& w, const Camera& cam) override;
	void setPlayerToHit(Soldier* s) {
		playerToHit=s;
	}

};

 
class MetalSlug :public GroundVehicle {
public:
static int metalSlugFireRate;
	MetalSlug(float x, float y, float wd, float ht, int hp, int fr, int vd) :GroundVehicle(x, y, wd, ht, hp, fr, vd) {
		texture.loadFromFile("25I-0504_25I-0644_Assets/metalslug.png");
		sprite.setTexture(texture);                      
	metalSlugFireRate=fr;}
	void attack()override;
	void move(float dt, const World& w)override;
	void update(float dt, const World& w)override;
	void render(sf::RenderWindow& w, const Camera& cam)override;

	int getFireRate()const {
		return normalFireRate;
	}
};
class AmphibiousSlug :public GroundVehicle {
float speedInWater;
float speedOnLand;
float speedInAir;
int currentForm;
public:
AmphibiousSlug(float x, float y, float wd, float ht, int hp, int fr, int vd):GroundVehicle(x,y,wd,ht,hp,fr,vd){
	texture.loadFromFile("25I-0504_25I-0644_Assets/amphibiouSlug.png");
	sprite.setTexture(texture);
speedInWater=100.0f;
speedOnLand=120.0f;
speedInAir=150.0f;
currentForm=0;
}
void attack()override;

void move(float dt,const World& w)override;
void update(float dt, const World& w)override;
void render(sf::RenderWindow& w, const Camera& cam)override;
void changeForm(const World& w);


};













