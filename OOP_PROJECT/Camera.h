#pragma once


class Camera {
public:
	float x = 0; 
	float y = 0;
	static const int screenW = 1600, screenH = 900;
	
	void follow(float x, float y);

	//check
	float toScreenX(float wx) const { return wx - x; }
	float toScreenY(float wy) const { return wy - y; }
};