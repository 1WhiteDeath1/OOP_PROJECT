#include "World.h"


World::World() {
	texture.loadFromFile("SPRITE/blocks/grass_block_side");

    //check
    for (int col = 0; col < WIDTH; col++) {
        setTile(11, col, 1, true, false);
        setTile(12, col, 1, true, false);
        setTile(13, col, 1, true, false);
    }

    // Platform — like their lvl[7][3..9]='#'
    for (int col = 20; col < 28; col++)
        setTile(8, col, 1, true, false);

    // Another platform
    for (int col = 40; col < 50; col++)
        setTile(6, col, 1, true, false);

}

void World::setTile(int row, int col, int type, bool solid, bool water) {
    Voxel& v = grid[row][col];
    v.type = type;
    v.blockX = col;
    v.blockY = row;
    v.isSolid = solid;
    v.isWater = water;
    v.sprite.setTexture(texture);
    v.sprite.setScale(
        (float)CELL / texture.getSize().x,
        (float)CELL / texture.getSize().y
    );
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