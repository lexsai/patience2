#ifndef PATIENCE_MAP_HPP
#define PATIENCE_MAP_HPP

#include "../renderer/renderer.hpp"

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

// using Map = std::vector<Tile>;
Map loadMap(std::string_view mapPath);

#endif
