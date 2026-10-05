#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// keeps track of the time of day. one full day is DAY_LENGTH seconds:
//   0  - 50  day
//   50 - 60  sunset   (gets darker)
//   60 - 110 night
//   110- 120 sunrise  (gets lighter)
class DayNightCycle
{
	float timeOfDay = 0;

public:
	static const int DAY_LENGTH = 120;

	void update(float dt);
	void setTime(float t) { timeOfDay = t; } // jump to a time of day (each mission starts at its own time)

	// 1 = full daylight, 0 = middle of the night
	float getBrightness() const;
	Color getSkyColor() const;

	// see-through dark layer drawn over the level, invisible in the day
	Color getNightOverlay() const;
	const char* getTimeName() const;

	// sun during the day half, moon during the night half, both move left to right in an arc
	void drawSunAndMoon(RenderWindow& window) const;
};
