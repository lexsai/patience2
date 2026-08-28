#include "map.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

std::map<int, Tile> tileById
{
  {0, Tile{}},
  {1, {{{64, 0}, {16, 16}}, false}},
  {2, {{{64, 80}, {16, 16}}, true}},
};

Map loadMap(std::string_view path)
{
  std::ifstream input{ std::string(path) };
  if (!input.is_open())
  {
    throw std::runtime_error("could not read file");
  }

  Map map{};
  input >> map.ground.width >> map.ground.height;
  map.ground.tiles.resize(
    map.ground.height, std::vector<Tile>(map.ground.width));

  for (int y = map.ground.height - 1; y >= 0; y--)
  {
    for (int x = 0; x < map.ground.width; x++)
    {
      int tileId;
      input >> tileId;
      map.ground.tiles[y][x] = tileById.at(tileId);
      std::cout << tileId;
    }
    std::cout << std::endl;
  }

  SDL_Log("dim %d %d", map.ground.width, map.ground.height);

  // throw std::runtime_error("lets stop here");
  return map;
}

bool isColliding(Tilemap& tilemap, Entity& e)
{
  int leftTile = static_cast<int>(e.left()) / TILE_WIDTH;
  int rightTile = static_cast<int>(e.right()) / TILE_WIDTH;
  int topTile = static_cast<int>(e.top()) / TILE_WIDTH;
  int bottomTile = static_cast<int>(e.bottom()) / TILE_WIDTH;

  if (leftTile < 0) leftTile = 0;
  if (rightTile < 0) rightTile = 0;
  if (topTile < 0) topTile = 0;
  if (bottomTile < 0) bottomTile = 0;

  if (leftTile >= tilemap.width) leftTile = tilemap.width - 1;
  if (rightTile >= tilemap.width) rightTile = tilemap.width - 1;
  if (topTile >= tilemap.height) topTile = tilemap.height - 1;
  if (bottomTile >= tilemap.height) topTile = tilemap.height - 1;

  for (int x = leftTile; x <= rightTile; x++)
  {
    for (int y = bottomTile; y <= topTile; y++)
    {
      Tile t = tilemap.tiles[y][x];
      if (t.solid)
      {
        return true;
      }
    }
  }
  return false;
}
