#pragma once
#include "Voxel.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>
using namespace sf;
// the world class manages hte physical layout of the level including the voxel grid. voxel is basically
// a tile either water, solid or empty.
class World
{
private:
	Texture stoneTex;
	Texture snowTex;
	Texture grassTex;
	Texture dirtTex;
	Texture sandTex;
	Texture waterTex;

	void setTile(int row, int col, int type, bool solid, bool water);

	void generateAerial();
	void generatePlains();
	void generateAquatic();
	int clamp(int val, int min, int max);

public:
	static const int CELL = 64;// each cell of grid is64x64pixels
	static const int HEIGHT = 40;
	static const int WIDTH = 200;

	static const int aerialEND = 66;
	static const int plainsEND = 133;
	static const int seaLEVEL = 28;

	static const int G = 10;

	Voxel grid[HEIGHT][WIDTH];

	World();
	void render(RenderWindow& w, const Camera& c);
	bool isSolid(float wX, float wY) const;
	bool isWater(float wX, float wY) const;

};

