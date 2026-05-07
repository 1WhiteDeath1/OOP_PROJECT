#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

struct Voxel {
	int type = 0;   // 0=empty 1=grass 2=water
	int blockX = 0;
	int blockY = 0;
	bool isSolid = false;
	bool isWater = false;
	Sprite sprite;
};
