#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Entity.h"
#include "World.h"

class DamagableEntity: public Entity
{
protected:
	int currentHp;
	int maxHp;
	bool touchingGround = false;
	const static int G = 1500;
	
public:
	DamagableEntity(float x, float y, float wd, float ht, int hp)
		: Entity(x, y, wd, ht), currentHp(hp), maxHp(hp) {
	}

	virtual void takeDamage(int amount);
	void applyGravity(float frameTime);
	void checkXCollisions(const World& w);
	void checkYCollisions(const World& w);
	void checkGroundCollisions(const World& w);

	virtual void die() { isActive = false; }

	bool isAlive() const { return currentHp > 0;}
	
	int getHp() const { return currentHp; }
	void heal(int amount) { currentHp += amount; if (currentHp > maxHp) currentHp = maxHp; }
	int getMaXHp() const { return maxHp; }
	void setPosition(float X, float Y);
	virtual ~DamagableEntity() = default;

};

