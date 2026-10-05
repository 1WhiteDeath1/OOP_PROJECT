#pragma once
#include <iostream>
#include <cmath>
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
	int hitFrames = 0; // frames left of the red "got hit" flash
	Uint8 alpha = 255; // see-through-ness of the sprite (255 = solid), used to fade out
	int muzzleFrames = 0; // frames left of the muzzle flash after shooting

	// a short yellow flash at the end of the gun (world position mx, my)
	void drawMuzzleFlash(RenderWindow& window, const Camera& cam, float mx, float my) {
		if (muzzleFrames <= 0) return;
		muzzleFrames--;
		CircleShape outer(11.f), inner(6.f);
		outer.setOrigin(11.f, 11.f); inner.setOrigin(6.f, 6.f);
		outer.setFillColor(Color(255, 150, 30, 220));
		inner.setFillColor(Color(255, 250, 200));
		outer.setPosition(cam.toScreenX(mx), cam.toScreenY(my));
		inner.setPosition(cam.toScreenX(mx), cam.toScreenY(my));
		window.draw(outer);
		window.draw(inner);
	}

	// tints the sprite red for a few frames after a hit, normal colour otherwise
	void applyHitTint() {
		if (hitFrames > 0) {
			sprite.setColor(Color(255, 90, 90, alpha));
			hitFrames--;
		}
		else sprite.setColor(Color(255, 255, 255, alpha));
	}
public:
	Entity(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}
	virtual void update(float dt, const World& w) = 0;
	virtual void render(RenderWindow& window, const Camera& cam) = 0;


	bool collision(const Entity& other) const {
		float myMidX = x + width / 2.f;
		float myMidY = y + height / 2.f;
		float otherMidX = other.x + other.width / 2.f;
		float otherMidY = other.y + other.height / 2.f;

		bool touchX = std::abs(myMidX - otherMidX)
			< ((width / 2.f + other.width / 2.f)-20);
		bool touchY = std::abs(myMidY - otherMidY)
			< ((height / 2.f + other.height / 2.f)-30);
		return touchX && touchY;
	}

	// the Metal Slug sprites are drawn 2.4x their original pixels, so a 40px tall soldier is 96px (1.5 blocks)
	// and every sprite keeps the same size relative to the others
	static constexpr float PIXEL_SCALE = 2.4f;

	// draws the sprite with the same scale on x and y (no stretching), standing on the bottom of the
	// hitbox and centred on it; mirrored flips it to face the other way.
	// lift raises it by some pixels and lean tilts it by some degrees (used for the walking bounce)
	void drawSprite(RenderWindow& window, const Camera& cam, float scale, bool mirrored, float lift = 0, float lean = 0) {
		FloatRect r = sprite.getLocalBounds();
		sprite.setOrigin(r.width / 2.f, r.height);
		sprite.setScale(mirrored ? -scale : scale, scale);
		sprite.setRotation(lean);
		sprite.setPosition(cam.toScreenX(x + width / 2.f), cam.toScreenY(y + height - lift));
		applyHitTint();
		window.draw(sprite);
	}

	// walking bounce worked out from how far the entity has walked (x), so it stops when it stands still:
	// a small hop every ~50px and a slight lean forward. returns false when not walking
	bool walkBounce(bool onGround, float& lift, float& lean) const {
		if (!onGround || std::abs(velocityX) < 30) { lift = 0; lean = 0; return false; }
		lift = std::abs(std::sin(x * 0.06f)) * 6;   // |sin| gives two hops per wave
		lean = velocityX > 0 ? 4.f : -4.f;          // tilt the top of the sprite the way it is walking
		return true;
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

