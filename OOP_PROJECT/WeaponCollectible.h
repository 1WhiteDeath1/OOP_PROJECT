#pragma once
#include "Weapon.h"
#include "Soldier.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>


class WeaponCollectible
{
	float x, y;
	const float WIDTH = 32;
	const float HIEGHT = 32;

	Texture tex;
	Sprite sprite;

	Weapon* weapon = nullptr;
	Soldier* pickUPGUY = nullptr;
	bool collected = false;
public:
	WeaponCollectible(float x, float y, Weapon* w, const char* texture) : x(x), y(y), weapon(w) {
		tex.loadFromFile(texture);
		sprite.setTexture(tex);
		sprite.setScale(WIDTH / tex.getSize().x, HIEGHT / tex.getSize().y);
	}

	~WeaponCollectible() {
		if (!collected) delete weapon;
	}

	bool isCollected() const { return collected; }

	bool collision(float playerX, float playerY, float playerWIDTH, float playerHEIGHT) const {
		float myMidX = x + WIDTH / 2.f;
		float myMidY = y + HIEGHT / 2.f;
		float otherMidX = playerX + playerWIDTH / 2.f;
		float otherMidY = playerY + playerHEIGHT / 2.f;

		bool touchX = abs(myMidX - otherMidX)
			< ((WIDTH / 2.f + playerWIDTH / 2.f) - 20.f);
		bool touchY = abs(myMidY - otherMidY)
			< ((HIEGHT / 2.f + playerHEIGHT / 2.f) - 30.f);
		return touchX && touchY;
	}

	void collect(Soldier* s) {
		s->setWeapon(1, weapon);
		weapon = nullptr;
		pickUPGUY = s;
		collected = true;
	}

	void update() {
		if (collected && pickUPGUY) {
			x = pickUPGUY->getX() + pickUPGUY->getWidth();
			y = pickUPGUY->getY();
		}
	}

	void render(RenderWindow& w, const Camera& cam) {
		sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
		w.draw(sprite);
	}
};

