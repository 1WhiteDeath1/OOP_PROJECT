#include "Weather.h"
#include "Camera.h"
#include <cstdlib>

void Weather::scatter() {
	for (int i = 0; i < COUNT; i++) {
		px[i] = (float)(rand() % Camera::screenW);
		py[i] = (float)(rand() % Camera::screenH);
	}
}


//rain: fast drops that fall slightly to the left
void Rain::update(float dt) {
	for (int i = 0; i < COUNT; i++) {
		px[i] -= 200 * dt;
		py[i] += 900 * dt;
		// gone off the bottom or the left: start again at the top
		if (py[i] > Camera::screenH || px[i] < 0) {
			px[i] = (float)(rand() % (Camera::screenW + 200));
			py[i] = -20;
		}
	}
}

void Rain::render(RenderWindow& window) {
	// every drop is a short line, all of them go in one vertex array so it is a single draw call
	VertexArray drops(Lines, COUNT * 2);
	for (int i = 0; i < COUNT; i++) {
		drops[i * 2].position = Vector2f(px[i], py[i]);
		drops[i * 2 + 1].position = Vector2f(px[i] + 6, py[i] - 28); // tail points back up the way it came
		drops[i * 2].color = Color(200, 215, 255, 230);
		drops[i * 2 + 1].color = Color(200, 215, 255, 90);
	}
	window.draw(drops);
}


// hardcoded schedule: how long each step lasts and which weather it is (0 = clear, 1 = rain)
static const int STEPS = 2;
static const int scheduleType[STEPS] = { 0, 1 };
static const float scheduleTime[STEPS] = { 30, 25 };

void WeatherSystem::startStep() {
	delete current;
	if (scheduleType[step] == 1) current = new Rain();
	else current = new ClearWeather();
	timeLeft = scheduleTime[step];
}

void WeatherSystem::update(float dt) {
	timeLeft -= dt;
	if (timeLeft <= 0) {
		step = (step + 1) % STEPS; // go to the next step, back to the start after the last one
		startStep();
	}
	current->update(dt);
}
