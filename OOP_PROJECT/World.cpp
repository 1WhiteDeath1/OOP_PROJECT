#include "World.h"
#include <random>

int World::clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}


World::World(int mission) {
    stoneTex.loadFromFile("25I-0504_25I-0644_Assets/stone.png");
    grassTex.loadFromFile("25I-0504_25I-0644_Assets/grass.png");
    dirtTex.loadFromFile("25I-0504_25I-0644_Assets/dirt.png");
    sandTex.loadFromFile("25I-0504_25I-0644_Assets/sand.png");
    waterTex.loadFromFile("25I-0504_25I-0644_Assets/water.png");

    if (mission == 1) {          // mountain pass: rocky hills, then plains
        generateAerial(0, 90);
        generatePlains(90, WIDTH);
    }
    else if (mission == 2) {     // coastline: plains, then the sea
        generatePlains(0, 110);
        generateAquatic(110, WIDTH);
    }
    else {                       // rebel base: flat desert, room to fight the boss
        generateDesert(0, WIDTH);
    }

}

void World::setTile(int row, int col, int type, bool solid, bool water) {
    Voxel& v = grid[row][col];// v refers to the same whole grid[row][col]
    v.type = type;
    v.blockX = col;
    v.blockY = row;
    v.isSolid = solid;
    v.isWater = water;

    switch (type) {
    case 1:
        v.sprite.setTexture(stoneTex); 
        break;
    case 2:
        v.sprite.setTexture(grassTex);
        break;
    case 3:
        v.sprite.setTexture(dirtTex);
        break;
    case 4:
        v.sprite.setTexture(sandTex);
        break;
    case 5:
        v.sprite.setTexture(waterTex);
        break;


   }



    v.sprite.setScale(
        (float)CELL / 64,
        (float)CELL / 64
    );
}








void World::generateAerial(int from, int to) {
    int surfaceHeight = 14;
    int flatLeft = 4; // columns left before the ground is allowed to step

    for (int i = from; i < to;i++) {
        // keep the ground flat for a few columns so every step is a ledge you can stand on,
        // then step 1 block (sometimes 2, still under the ~2.5 block jump height)
        if (flatLeft == 0) {
            int rigidness = (rand() % 2 == 0) ? 1 : -1;
            if (rand() % 4 == 0) rigidness *= 2;
            // near the end, stair down towards the plains so the two biomes meet with a normal step
            if (i > to - 20) rigidness = (surfaceHeight < 16) ? 2 : 1;
            surfaceHeight += rigidness;
            surfaceHeight = clamp(surfaceHeight, 9, 18);
            flatLeft = 3 + rand() % 3; // 3 to 5 columns wide
        }
        flatLeft--;

        for (int j = surfaceHeight; j< HEIGHT;j++) {
            setTile(j, i, 1, true, false);
        }
    }
}

void World::generatePlains(int from, int to) {
    int surfaceHeight = 20;
    int flatLeft = 5;

    for (int i = from; i < to;i++) {
        // plains are gentler: long flat stretches with 1 block steps
        if (flatLeft == 0) {
            int rigidness = (rand() % 2 == 0) ? 1 : -1;
            surfaceHeight += rigidness;
            surfaceHeight = clamp(surfaceHeight, 18, 23);
            flatLeft = 4 + rand() % 4; // 4 to 7 columns wide
        }
        flatLeft--;

        //grass on the surface, and beneath it dirt, smarty ants

        setTile(surfaceHeight, i, 2, true, false);

        for (int j = surfaceHeight+1; j < HEIGHT;j++) {
            setTile(j, i, 3, true, false);
        }
    }
}


void World::generateAquatic(int from, int to) {
    int bed = 34;
    int flatLeft = 4;

    for (int i = from; i < to;i++) {
        // sea bed rises and falls in wide 1 block steps instead of a new random depth every column
        if (flatLeft == 0) {
            bed += (rand() % 2 == 0) ? 1 : -1;
            bed = clamp(bed, 33, HEIGHT - 3);
            flatLeft = 3 + rand() % 4; // 3 to 6 columns wide
        }
        flatLeft--;

        for (int j = bed; j < HEIGHT;j++) {
            setTile(j, i, 4, true, false);
        }

        for (int j = seaLEVEL; j < bed; j++) {
            setTile(j, i, 5, false, true);
        }
    }
}









void World::generateDesert(int from, int to) {
    int surfaceHeight = 21;
    int flatLeft = 12;

    for (int i = from; i < to; i++) {
        // mostly flat sand with an odd 1 block bump, so the boss fight has room
        if (flatLeft == 0) {
            surfaceHeight = (surfaceHeight == 21) ? 20 : 21;
            flatLeft = (surfaceHeight == 20) ? 3 + rand() % 3 : 10 + rand() % 10;
        }
        flatLeft--;

        for (int j = surfaceHeight; j < HEIGHT; j++) {
            setTile(j, i, 4, true, false);
        }
    }
}

void World::render(RenderWindow& w, const Camera& cam) {
    for (int i = 0; i <HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].type == 0) continue;
            float screeX = cam.toScreenX(j * CELL);
            float screeY = cam.toScreenY(i * CELL);

            if (screeX > -CELL && screeX<Camera::screenW && screeY > -CELL && screeY < Camera::screenH) {
                grid[i][j].sprite.setPosition(screeX, screeY);
                w.draw(grid[i][j].sprite);
            }
        }
    }
}

float World::surfaceY(int col) const {
    for (int row = 0; row < HEIGHT; row++) {
        if (grid[row][col].isSolid) return (float)(row * CELL);
    }
    return (float)(HEIGHT * CELL);
}

//check
bool World::isSolid(float wx, float wy) const {
    int col = (int)(wx / CELL);
    int row = (int)(wy / CELL);
    if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH) return false;
    return grid[row][col].isSolid;
}

bool World::isWater(float wx, float wy) const {
    int col = (int)(wx / CELL);
    int row = (int)(wy / CELL);
    if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH) return false;
    return grid[row][col].isWater;
}