#pragma once


class Camera {
public:
	float x = 0; 
	float y = 0;
	static const int screenW = 1600, screenH = 900;
	
	void follow(float wx, float wy) {
		x = wx - screenW / 2;
		y = wy - screenH / 2;
	}

	//check
	float toScreenX(float wx) const { return wx - x; }
	float toScreenY(float wy) const { return wy - y; }
};