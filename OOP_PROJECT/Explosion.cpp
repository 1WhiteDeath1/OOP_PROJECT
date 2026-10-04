#include "Explosion.h"

Texture Explosion::tex;
bool Explosion::texIsLoaded = false;

void Explosion::start(float X, float Y, float height) {
	if (!texIsLoaded) {
		tex.loadFromFile("25I-0504_25I-0644_Assets/explosion.png");
		texIsLoaded = true;
	}
	sprite.setTexture(tex);
	x = X;
	y = Y;
	size = height;
	timer = 0;
	active = true;
}

void Explosion::update(float dt) {
	if (!active) return;
	timer += dt;
	if (timer >= DURATION) active = false; // animation finished
}

void Explosion::render(RenderWindow& window, const Camera& cam) {
	if (!active) return;

	// which picture of the strip to show: goes from 0 to FRAMES-1 over DURATION seconds
	int frame = (int)(timer / DURATION * FRAMES);
	if (frame >= FRAMES) frame = FRAMES - 1;

	int frameW = tex.getSize().x / FRAMES;
	int frameH = tex.getSize().y;
	sprite.setTextureRect(IntRect(frame * frameW, 0, frameW, frameH)); // only show that one picture

	float scale = size / frameH;
	sprite.setOrigin(frameW / 2.f, (float)frameH); // bottom centre, so the fire grows up from where it hit
	sprite.setScale(scale, scale);
	sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
	window.draw(sprite);
}
