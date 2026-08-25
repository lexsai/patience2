#ifndef PATIENCE_GAME_HPP
#define PATIENCE_GAME_HPP

#include <vector>
#include "renderer.hpp"

struct UserCommand {
  bool forward;
  bool back;
  bool left;
  bool right;
};

struct EntityId
{
  int generation;
  int index;
};

class Game;

struct Entity
{
  // EntityId entity_id{};
  float m_x;
  float m_y;
};

class Game
{
  int m_nextGeneration{};
  std::vector<Entity> m_entities{};
  // m_colliders

public:
  Game();
  ~Game();

  void update(UserCommand& userCmd, Renderer& r);
  void updateUserCmd(UserCommand& userCmd, SDL_Scancode keyCode, bool isDown);

  void createEntity(float x, float y);
};

#endif
