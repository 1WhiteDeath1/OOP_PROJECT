#pragma once
#include <cstdlib>


class Camera {
	float shakeTime = 0;     // seconds of shaking left
	float shakeStrength = 0; // how many pixels it can jump
	float offX = 0, offY = 0;
public:
	float x = 0; 
	float y = 0;
	static const int screenW = 1600, screenH = 900;
	
	void follow(float wx, float wy) {
		x = wx - screenW / 2;
		y = wy - screenH / 2;
	}

	// start shaking (a stronger shake replaces a weaker one)
	void shake(float strength, float time) {
		if (strength >= shakeStrength || shakeTime <= 0) {
			shakeStrength = strength;
			shakeTime = time;
		}
	}
	// every frame: pick a new random offset while shaking, the shake fades out as time runs down
	void updateShake(float dt) {
		if (shakeTime > 0) {
			shakeTime -= dt;
			float s = shakeStrength * (shakeTime > 0.2f ? 1.f : shakeTime / 0.2f);
			offX = ((rand() % 201) - 100) / 100.f * s;
			offY = ((rand() % 201) - 100) / 100.f * s;
		}
		else { offX = 0; offY = 0; shakeStrength = 0; }
	}

	//check
	float toScreenX(float wx) const { return wx - x + offX; }
	float toScreenY(float wy) const { return wy - y + offY; }
};
