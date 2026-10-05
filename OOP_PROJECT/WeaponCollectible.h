#pragma once
#include "Weapon.h"
#include "Soldier.h"
#include "Camera.h"
#include "SoundManager.h"
#include <SFML/Graphics.hpp>


class WeaponCollectible
{
	float x, y;
	const float WIDTH = 48;
	const float HIEGHT = 32;

	Texture tex;
	Sprite sprite;

	Weapon* weapon = nullptr;
	Weapon* given = nullptr; // the weapon handed to the soldier, used to know when it was replaced
	Soldier* pickUPGUY = nullptr;
	bool collected = false;
public:
	WeaponCollectible(float x, float y, Weapon* w, const char* texture) : x(x), y(y), weapon(w) {
		tex.loadFromFile(texture);
		sprite.setTexture(tex);
		if (tex.getSize().x > 0) {
			// fit the picture inside the pickup box
			float sx = WIDTH / tex.getSize().x, sy = HIEGHT / tex.getSize().y;
			float s = sx < sy ? sx : sy;
			sprite.setScale(s, s);
		}
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

		bool touchX = std::abs(myMidX - otherMidX)
			< ((WIDTH / 2.f + playerWIDTH / 2.f) );
		bool touchY = std::abs(myMidY - otherMidY)
			< ((HIEGHT / 2.f + playerHEIGHT / 2.f));
		return touchX && touchY;
	}

	void collect(Soldier* s) {
		s->setWeapon(1, weapon);
		SoundManager::play(SoundManager::PICKUP);
		given = weapon;
		weapon = nullptr;
		pickUPGUY = s;
		collected = true;
	}

	void update() {
		if (collected && pickUPGUY) {
			x = pickUPGUY->getX() + pickUPGUY->getWidth();
			y = pickUPGUY->getY() + pickUPGUY->getHeight() / 2.f - HIEGHT / 2.f;
		}
	}

	void render(RenderWindow& w, const Camera& cam) {

		//checking for loss of ammo
		if (collected && pickUPGUY) {
			Weapon* slot1 = pickUPGUY->getActiveWeaponSlot(1);
			if (!slot1 || slot1 != given || !slot1->hasAmmo() || !pickUPGUY->getActive()) return;
		}



		sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
		w.draw(sprite);
	}
};

