#include "DayNightCycle.h"

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
