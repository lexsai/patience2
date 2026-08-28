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
