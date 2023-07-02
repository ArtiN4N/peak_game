#include <../include/raylib.h>
#include <../include/map.h>

Tile::Tile(TileType tp, int dmg, bool pog, int hp) {
    type = tp;

    source = { 0, 0, 0, 0 };
    
    damage = dmg;
    health = hp;

    pogo = pog;
    active = true;
}

void Map::writeMap(std::string file) {
    std::ifstream mapData(file);
    int t = 0;

    while (mapData) {
        // Read each line of the map data.
        std::string line;
        
        if (!getline(mapData, line)) break;

        std::istringstream mapStream(line);
        
        while (mapStream) {
            std::string tile;
            // Split map data into comma seperated list.
            if (!getline(mapStream, tile, ',')) break;

            // Map data has tile names seperated by commas.
            // Each tile name has a map to a tile.
            // We take that tile, and add a pointer to it to the tileMap.
            tileMap[t] = &tiles[stoi(tile)];
            t++;
            if (t >= width * height) break;
        }
    }
}

void Map::createTiles() {
    float x = 0;
    Tile wall1 = Tile(STATIC);
    wall1.source = {x, 0, 32, 32};
    x+=32;
    Tile wall2 = Tile(STATIC);
    wall2.source = {x, 0, 32, 32};
    x+=32;
    Tile wall3 = Tile(STATIC);
    wall3.source = {x, 0, 32, 32};
    x+=32;
    Tile wall4 = Tile(STATIC);
    wall4.source = {x, 0, 32, 32};
    x+=32;
    Tile wall5 = Tile(STATIC);
    wall5.source = {x, 0, 32, 32};
    x+=32;
    Tile wall6 = Tile(STATIC);
    wall6.source = {x, 0, 32, 32};
    x+=32;
    Tile wall7 = Tile(STATIC);
    wall7.source = {x, 0, 32, 32};
    x+=32;
    Tile wall8 = Tile(STATIC);
    wall8.source = {x, 0, 32, 32};
    x+=32;
    Tile wall9 = Tile(STATIC);
    wall9.source = {x, 0, 32, 32};
    x+=32;

    Tile air = Tile(NONE);
    air.source = {x, 0.f, 32.f, 32.f};
    Tile transition = Tile(TRANSITION);
    transition.source = {x, 0.f, 32.f, 32.f}; 
    x+= 32;
    Tile bonfire = Tile(BONFIRE);
    bonfire.source = {x, 0.f, 32.f, 32.f}; 
    x+=32;
    Tile spike = Tile(DAMAGE, 1, true);
    spike.source = {x, 0.f, 32.f, 32.f}; 
    x+= 32;
    Tile shop = Tile(SHOP);
    shop.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile ladder = Tile(LADDER);
    ladder.source = {x, 0.f, 32.f, 32.f};
    x+= 32;

    Tile z = Tile(BACKGROUND);
    z.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile xKey = Tile(BACKGROUND);
    xKey.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile c = Tile(BACKGROUND);
    c.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile up = Tile(BACKGROUND);
    up.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile left = Tile(BACKGROUND);
    left.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile down = Tile(BACKGROUND);
    down.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile right = Tile(BACKGROUND);
    right.source = {x, 0.f, 32.f, 32.f};
    x+= 32;
    Tile plus = Tile(BACKGROUND);
    plus.source = {x, 0.f, 32.f, 32.f};
    
    tiles[1] = wall1;
    tiles[2] = wall2;
    tiles[3] = wall3;
    tiles[4] = wall4;
    tiles[5] = wall5;
    tiles[6] = wall6;
    tiles[7] = wall7;
    tiles[8] = wall8;
    tiles[9] = wall9;

    tiles[10] = air;
    tiles[11] = bonfire;
    tiles[12] = spike;
    tiles[13] = shop;
    tiles[14] = ladder;
    tiles[15] = transition;

    tiles[16] = z;
    tiles[17] = xKey;
    tiles[18] = c;
    tiles[19] = up;
    tiles[20] = left;
    tiles[21] = down;
    tiles[22] = right;
    tiles[23] = plus;
}

Map::Map() {
    instantiated = false;

    width = 0;
    height = 0;

    tileSize = 55;
    startTile = { 0, 0 };
    bonfireCoords = { 0, 0 };

    tileSheet = loadTextureUnloadImage("assets/tileSheet.png");
    createTiles();
}

void Map::kill() {
    if (instantiated) free(tileMap);

    UnloadTexture(tileSheet);
    instantiated = false;
}

void Map::reset() {
    if (instantiated) free(tileMap);
    instantiated = false;
    // do something
}

Tile* Map::getTile(int x, int y) {
    return tileMap[x * height + y];
}

void Map::draw(Camera2D camera) {

    int y = (camera.target.y - camera.offset.y) / tileSize;
    int maxY = (camera.target.y + camera.offset.y) / tileSize;

    int x = (camera.target.x - camera.offset.x) / tileSize;
    int maxX = (camera.target.x + camera.offset.x) / tileSize;

    clamp(x, 0, width - 1);
    clamp(y, 0, height - 1);

    clamp(maxX, 0, width - 1);
    clamp(maxY, 0, height - 1);

    for (; y < maxY + 1; y++) {
        for (int i = x; i < maxX + 1; i++) {
            if (!(getTile(i, y)->type == NONE || getTile(i, y)->type == TRANSITION)) {
                Rectangle src = getTile(i, y)->source;
                Rectangle dest = { i * tileSize, y * tileSize, tileSize, tileSize };

                float rotation = 0.f;
                if (i > 0) {
                    if (getTile(i - 1, y)->type == DAMAGE) {
                        if (getTile(i - 1, y)->type == STATIC) rotation = 3 * PI / 2;
                        if (getTile(i + 1, y)->type == STATIC) rotation = PI / 2;
                        if (getTile(i, y - 1)->type == STATIC) rotation = PI;
                    }
                }
                
                
                DrawTexturePro(tileSheet, src, dest, ZERO, rotation, WHITE);
            }
        }
    }
}

void Map::assignMap(std::string mapName, int w, int h, Vector2 sT, Vector2 bC) {
    if (instantiated) free(tileMap);
    instantiated = true;

    width = w;
    height = h;
    startTile = sT;
    bonfireCoords = bC;

    tileMap = (Tile**) malloc(width * height * sizeof(Tile*));

    // Init map with error tiles, in case loading fails.
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            tileMap[x * height + y] = &(tiles[0]);
        }
    }

    writeMap(mapName);
}
