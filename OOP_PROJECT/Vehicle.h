#pragma once
#include "DamagableEntity.h"
#include "Soldier.h"
class Soldier;

class Vehicle :public DamagableEntity {
protected:
int normalFireRate;
float currentTime=0;

int vehicleDurability;
bool playerInVehicle;
Soldier* playerInside=nullptr;
Projectile* projectile=nullptr;

public:
Vehicle(float x, float y, float wd, float ht, int hp,int Fr, int vd):DamagableEntity(x,y,wd,ht,hp),normalFireRate(Fr),vehicleDurability(vd),playerInVehicle(false){
playerInside=nullptr;
}



virtual void attack()=0;
virtual void move(float dt, const World&w)=0;// we need world for collision and dt so game runs smoothly irrespective of fps
virtual void update(float dt, const World&w)=0;
virtual void render(sf::RenderWindow&w, const Camera&cam)=0;

// take damage and die functions are in damagableEntity
void enterVehicle(Soldier* soldier) {
	if(soldier==nullptr)
	return ;
	playerInside=soldier;
	playerInVehicle=true;
	playerInside->setActive(false);
	playerInside->setPosition(x,y);//+10 so player appears after vehicle not on it

}
void exitVehicle() {
if(playerInside==nullptr)
return;
playerInVehicle=false;
playerInside->setActive(true);
playerInside->setPosition(x+10,y);
playerInside=nullptr;

}
bool isVehicleOccupied()const {
	return playerInVehicle;
}
bool playerVehicle()const {
if(playerInside==nullptr)
return false;
else
return true;

}
Soldier* getPlayer()const {
	return playerInside;
}
virtual ~Vehicle(){}

};



