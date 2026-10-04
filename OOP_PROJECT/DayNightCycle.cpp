#include "DayNightCycle.h"
#include <cmath>

void DayNightCycle::update(float dt) {
	timeOfDay += dt;
	if (timeOfDay >= DAY_LENGTH) timeOfDay -= DAY_LENGTH; // start the next day
}

float DayNightCycle::getBrightness() const {
	if (timeOfDay < 50) return 1;                            // day
	if (timeOfDay < 60) return 1 - (timeOfDay - 50) / 10.f;  // sunset, 1 -> 0 over 10 seconds
	if (timeOfDay < 110) return 0;                           // night
	return (timeOfDay - 110) / 10.f;                         // sunrise, 0 -> 1 over 10 seconds
}

Color DayNightCycle::getSkyColor() const {
	// mix between the night sky colour and the day sky colour by the brightness
	float b = getBrightness();
	Color day(120, 190, 235);
	Color night(15, 20, 45);
	return Color(
		(Uint8)(night.r + (day.r - night.r) * b),
		(Uint8)(night.g + (day.g - night.g) * b),
		(Uint8)(night.b + (day.b - night.b) * b)
	);
}


Color DayNightCycle::getNightOverlay() const {
	// alpha 0 = fully see-through (day), 140 = dark blue tint at night (still playable)
	return Color(10, 10, 40, (Uint8)(140 * (1 - getBrightness())));
}

const char* DayNightCycle::getTimeName() const {
	if (timeOfDay < 50) return "Day";
	if (timeOfDay < 60) return "Sunset";
	if (timeOfDay < 110) return "Night";
	return "Sunrise";
}

void DayNightCycle::drawSunAndMoon(RenderWindow& window) const {
	// first half of the day (0-60) belongs to the sun, second half (60-120) to the moon
	float half = DAY_LENGTH / 2.f;
	bool isSun = timeOfDay < half;
	float progress = isSun ? timeOfDay / half : (timeOfDay - half) / half; // 0 = rises on the left, 1 = sets on the right

	float x = 100 + progress * 1400;
	float y = 420 - std::sin(progress * 3.14159f) * 300; // highest in the middle of the screen

	CircleShape body(isSun ? 45.f : 32.f);
	body.setOrigin(body.getRadius(), body.getRadius());
	body.setPosition(x, y);
	body.setFillColor(isSun ? Color(255, 225, 90) : Color(235, 235, 210));
	window.draw(body);
}
