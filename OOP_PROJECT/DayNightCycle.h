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

	// 1 = full daylight, 0 = middle of the night
	float getBrightness() const;
	Color getSkyColor() const;

	// see-through dark layer drawn over the level, invisible in the day
	Color getNightOverlay() const;
	const char* getTimeName() const;
};
