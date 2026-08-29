#ifndef PATIENCE_MAP_HPP
#define PATIENCE_MAP_HPP

#include "../renderer/renderer.hpp"
#include "entities.hpp"

#include <vector>

struct Tile {
  int id;
  SpriteAtlasSpecifier sprite;
  bool solid;
};

struct Tilemap
{
  std::vector<std::vector<Tile>> tiles;
  int height;
  int width;

  Tile* get(float worldX, float worldY)
  {
    int xTile = static_cast<int>(worldX) / TILE_WIDTH;
    int yTile = static_cast<int>(worldY) / TILE_WIDTH;
    
    if (xTile < 0) xTile = 0;
    if (xTile >= width) xTile = width - 1;

    if (yTile < 0) yTile = 0;
    if (yTile >= height) yTile = height - 1;

    return &tiles[yTile][xTile];
  }
};

struct Map {
  Tilemap ground;
  Tilemap ceiling;
};

Map loadMap(std::string_view mapPath);
void exportMap(Map& map);
bool isColliding(Tilemap& tilemap, Entity& e);
glm::vec2 worldPosToTilePos(float wX, float wY);

extern std::map<int, Tile> tileById;

#endif
