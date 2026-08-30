#include "map.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

glm::vec2 worldPosToTilePos(float wX, float wY)
{
  int tX = static_cast<int>(wX) / TILE_WIDTH;
  int tY = static_cast<int>(wY) / TILE_WIDTH;

  return glm::vec2(tX * static_cast<float>(TILE_WIDTH), 
    tY * static_cast<float>(TILE_WIDTH));
}

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

void exportMap(Map& map, std::string_view path)
{
  std::ofstream output{ std::string(path) };
  if (!output.is_open())
  {
    throw std::runtime_error("could not read file");
  }

  output << map.ground.width << " " << map.ground.height << "\n\n";

  for (int y = map.ground.height - 1; y >= 0; y--)
  {
    for (int x = 0; x < map.ground.width; x++)
    {
      output << map.ground.tiles[y][x].id << " ";
    }
    output << "\n";
  }

  output << "\n";

  for (int y = map.ceiling.height - 1; y >= 0; y--)
  {
    for (int x = 0; x < map.ceiling.width; x++)
    {
      output << map.ceiling.tiles[y][x].id << " ";
    }
    output << "\n";
  }
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

std::map<int, Tile> tileById
{
  {0, Tile{}},
  {1, {1, {{64, 0}, {16, 16}}, false}},
  {2, {2, {{64, 80}, {16, 16}}, true}},
};
