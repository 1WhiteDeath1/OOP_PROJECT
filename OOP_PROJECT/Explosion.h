#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
using namespace sf;

// a short fireball animation, played where a grenade or rocket goes off or a vehicle is destroyed.
// explosion.png is a strip of FRAMES pictures side by side, played left to right
class Explosion
{
	static Texture tex;
	static bool texIsLoaded;
	static const int FRAMES = 10;

	Sprite sprite;
	float x = 0, y = 0;     // bottom centre of the fireball in the world
	float size = 0;         // how tall it is drawn, in pixels
	float timer = 0;
	bool active = false;
public:
	static constexpr float DURATION = 0.6f; // seconds for the whole animation

	void start(float X, float Y, float height);
	void update(float dt);
	void render(RenderWindow& window, const Camera& cam);
	bool isActive() const { return active; }
};
