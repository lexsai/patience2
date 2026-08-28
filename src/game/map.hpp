#ifndef PATIENCE_MAP_HPP
#define PATIENCE_MAP_HPP

#include "../renderer/renderer.hpp"
#include "entities.hpp"

#include <vector>

struct Tile {
  SpriteAtlasSpecifier sprite;
  bool solid;
};

struct Tilemap
{
  std::vector<std::vector<Tile>> tiles;
  int height;
  int width;
};

struct Map {
  Tilemap ground;
  Tilemap ceiling;
};

Map loadMap(std::string_view mapPath);
bool isColliding(Tilemap& tilemap, Entity& e);

#endif
