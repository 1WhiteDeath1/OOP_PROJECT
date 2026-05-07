#pragma once
#include "Voxel.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class World
{
private:
	Texture texture;
	void setTile(int row, int col, int type, bool solid, bool water);

public:
	static const int CELL = 64;
	static const int HEIGHT = 14;
	static const int WIDTH = 110;
	static const int G = 10;

	Voxel grid[HEIGHT][WIDTH];

	World();
	void render(RenderWindow& w, const Camera& c);
	bool isSolid(float wX, float wY) const;
	bool isWater(float wX, float wY) const;

};

