#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "World.h"
#include "Camera.h"

using namespace sf;

class Entity
{
protected:
	float x, y;
	float velocityX = 0;
	float velocityY = 0;
	Sprite sprite;
	Texture texture;
	float width, height;
	bool isActive = true;
public:
	Entity(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}
	virtual void update(float dt, const World& w) = 0;
	virtual void render(RenderWindow& window, const Camera& cam) = 0;


	bool collision(const Entity& other) const {
		float myMidX = x + width / 2.f;
		float myMidY = y + height / 2.f;
		float otherMidX = other.x + other.width / 2.f;
		float otherMidY = other.y + other.height / 2.f;

		bool touchX = abs(myMidX - otherMidX)
			< ((width / 2.f + other.width / 2.f) - 20.f);
		bool touchY = abs(myMidY - otherMidY)
			< ((height / 2.f + other.height / 2.f) - 30.f);
		return touchX && touchY;
	}

	bool isTouchingBlock(const World& w) const;

	// More specific tile checks — subclasses use whichever they need
	bool isTouchingGround(const World& w) const;   // feet only
	bool isTouchingCeiling(const World& w) const;  // head only
	bool isTouchingLeftWall(const World& w) const;  // left edge only
	bool isTouchingRightWall(const World& w) const; // right edge only


	virtual ~Entity() = default;

	float getX()      const { return x; }
	float getY()      const { return y; }
	float getWidth()  const { return width; }
	float getHeight() const { return height; }
	bool  getActive() const { return isActive; }
	void  setActive(bool a) { isActive = a; }
	
};

