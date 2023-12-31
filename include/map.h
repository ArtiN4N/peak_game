#ifndef MAP_H_
#define MAP_H_

#include <raylib.h>
#include <collider.h>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <utility>


enum TileType { STATIC = 0, DAMAGE, BACKGROUND, BONFIRE, TRANSITION, SHOP, LADDER, NONE };

struct Tile {
    TileType type;

    Rectangle source;

    int damage;
    int health;

    bool pogo;
    bool active;

    Tile(TileType tp = NONE, int dmg = 0, bool pog = false, int hp = 10);
};

struct Map {
    bool instantiated;

    float tileSize;
    int width;
    int height;
    Vector2 startTile;

    Tile** tileMap;
    std::map<int, Tile> tiles;
    Texture2D tileSheet;
    std::string mapName;

    Vector2 bonfireCoords;

    Map();

    void writeMap(std::string file);
    void createTiles();

    void kill();
    void reset();

    Tile* getTile(int x, int y);

    void assignMap(std::string mapName, int w, int h, Vector2 sT, Vector2 bC = ZERO);

    void draw(Camera2D camera);
};


#endif