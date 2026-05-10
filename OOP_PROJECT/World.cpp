#include "World.h"
#include <random>

int World::clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}


World::World() {
    stoneTex.loadFromFile("Sprites/stone.png");
    grassTex.loadFromFile("Sprites/grass.png");
    dirtTex.loadFromFile("Sprites/dirt.png");
    sandTex.loadFromFile("Sprites/sand.png");
    waterTex.loadFromFile("Sprites/water.png");

    generateAerial();
    generatePlains();
    generateAquatic();

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
        (float)CELL / v.sprite.getTexture()->getSize().x,
        (float)CELL / v.sprite.getTexture()->getSize().y
    );
}








void World::generateAerial() {
    int surfaceHeight = 8;
    for (int i = 0; i < aerialEND;i++) {
        int rigidness = (rand() % 7) - 3;
        surfaceHeight += rigidness;
        surfaceHeight = clamp(surfaceHeight, 3, 18);

        for (int j = surfaceHeight; j< HEIGHT;j++) {
            setTile(j, i, 1, true, false);
        }
    }
}

void World::generatePlains() {
    int surfaceHeight = 20;

    for (int i = aerialEND; i < plainsEND;i++) {
        int rigidness = (rand() % 3) - 1;
        surfaceHeight += rigidness;
        surfaceHeight = clamp(surfaceHeight, 17, 25);

        //grass on the surface, and beneath it dirt, smarty ants

        setTile(surfaceHeight, i, 2, true, false);

        for (int j = surfaceHeight+1; j < HEIGHT;j++) {
            setTile(j, i, 3, true, false);
        }
    }
}


void World::generateAquatic() {
    for (int i = plainsEND; i < WIDTH;i++) {
       
        int bed = 33 + (rand() % 4);
        bed = clamp(bed, 33, HEIGHT - 2);

        for (int j = bed; j < HEIGHT;j++) {
            setTile(j, i, 4, true, false);
        }

        for (int j = seaLEVEL; j < bed; j++) {
            setTile(j, i, 5, false, true);
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