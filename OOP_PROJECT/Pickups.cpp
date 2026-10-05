#include "Pickups.h"

// both kinds of pickup fall onto the ground the same simple way
static void fallToGround(float& y, float& velocityY, float x, float w, float h, float dt, const World& world) {
	velocityY += 900 * dt;
	if (velocityY > 600) velocityY = 600;
	y += velocityY * dt;
	if (world.isSolid(x + w / 2, y + h)) {
		y = (float)((int)((y + h) / World::CELL) * World::CELL) - h; // stand on top of the block
		velocityY = 0;
	}
}

// item drops

Texture ItemDrop::tex[ItemDrop::TYPE_COUNT];
bool ItemDrop::texIsLoaded = false;

ItemDrop::ItemDrop(float x, float y, Type type) : Entity(x, y, 40, 30), type(type) {
	if (!texIsLoaded) {
		tex[AMMO].loadFromFile("25I-0504_25I-0644_Assets/drop_ammo.png");
		tex[GRENADES].loadFromFile("25I-0504_25I-0644_Assets/drop_grenades.png");
		tex[HEALTH].loadFromFile("25I-0504_25I-0644_Assets/drop_health.png");
		texIsLoaded = true;
	}
	sprite.setTexture(tex[type]);
	velocityY = -250; // pops up a little before falling
}

void ItemDrop::update(float dt, const World& w) {
	fallToGround(y, velocityY, x, width, height, dt, w);
	life -= dt;
	if (life <= 0) isActive = false;
}

void ItemDrop::render(RenderWindow& window, const Camera& cam) {
	if (life < 3 && (int)(life * 8) % 2 == 0) return; // blink in the last 3 seconds
	drawSprite(window, cam, 1.6f, false);
}

// prisoners

Texture Prisoner::tiedTex;
Texture Prisoner::runTex;
bool Prisoner::texIsLoaded = false;

Prisoner::Prisoner(float x, float y) : Entity(x, y, 64, 70) {
	if (!texIsLoaded) {
		tiedTex.loadFromFile("25I-0504_25I-0644_Assets/pow_tied.png");
		runTex.loadFromFile("25I-0504_25I-0644_Assets/pow_run.png");
		texIsLoaded = true;
	}
	sprite.setTexture(tiedTex);
}

void Prisoner::update(float dt, const World& w) {
	if (!freed) {
		fallToGround(y, velocityY, x, width, height, dt, w);
		return;
	}
	// freed: run away to the left for 2 seconds, then he is gone
	runTimer += dt;
	x -= 220 * dt;
	if (runTimer > 2) isActive = false;
}

void Prisoner::render(RenderWindow& window, const Camera& cam) {
	if (!freed) {
		sprite.setTexture(tiedTex, true);
	}
	else {
		sprite.setTexture(runTex, true);
		int frameW = runTex.getSize().x / RUN_FRAMES;
		int frame = (int)(runTimer * 14) % RUN_FRAMES;
		sprite.setTextureRect(IntRect(frame * frameW, 0, frameW, runTex.getSize().y));
	}
	drawSprite(window, cam, PIXEL_SCALE, false);
}
