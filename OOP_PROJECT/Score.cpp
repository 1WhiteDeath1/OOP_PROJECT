#include "Score.h"
#include <fstream>
#include <string>

Score::Score(int startPoints) : points(startPoints) {
	for (int i = 0; i < MAX_POPUPS; i++) popups[i].timer = 0;
}

void Score::add(int basePoints, float worldX, float worldY) {
	// still inside the combo time: the combo grows, otherwise it starts again at 1
	combo = (comboTimer > 0) ? combo + 1 : 1;
	comboTimer = COMBO_TIME;

	int value = basePoints * getMultiplier();
	points += value;

	// show the points where it happened (reuse a finished popup)
	for (int i = 0; i < MAX_POPUPS; i++) {
		if (popups[i].timer <= 0) {
			popups[i] = { worldX, worldY, 1.0f, value };
			break;
		}
	}
}

void Score::update(float dt) {
	if (comboTimer > 0) comboTimer -= dt;
	else combo = 0;
	for (int i = 0; i < MAX_POPUPS; i++) {
		if (popups[i].timer > 0) {
			popups[i].timer -= dt;
			popups[i].y -= 60 * dt; // float upwards
		}
	}
}

void Score::renderPopups(RenderWindow& w, const Camera& cam, Text& text) {
	for (int i = 0; i < MAX_POPUPS; i++) {
		if (popups[i].timer <= 0) continue;
		text.setString("+" + std::to_string(popups[i].value));
		Color c(255, 230, 80, (Uint8)(255 * (popups[i].timer > 0.3f ? 1 : popups[i].timer / 0.3f))); // fades at the end
		text.setFillColor(c);
		text.setOutlineColor(Color(0, 0, 0, c.a));
		FloatRect b = text.getLocalBounds();
		text.setOrigin(b.width / 2, b.height);
		text.setPosition(cam.toScreenX(popups[i].x), cam.toScreenY(popups[i].y));
		w.draw(text);
	}
}

int Score::loadHighScore() {
	std::ifstream file("highscore.txt");
	int best = 0;
	if (file >> best) return best;
	return 0; // no file yet
}

void Score::saveHighScore(int value) {
	if (value <= loadHighScore()) return;
	std::ofstream file("highscore.txt");
	file << value;
}
