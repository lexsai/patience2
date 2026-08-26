#ifndef PATIENCE_GAME_HPP
#define PATIENCE_GAME_HPP

#include <vector>

#include "../renderer/renderer.hpp"
#include "entities.hpp"

struct UserCommand {
  bool forward;
  bool back;
  bool left;
  bool right;
  bool test;
};

class Game
{
  int m_nextGeneration{};
  // m_colliders

  void updateEntityLogic(UserCommand& userCmd);
  void updateSystems();

  void drawEntities(Renderer& r);
  void drawHUD(Renderer& r);
public:
  std::vector<Entity> m_entities{};
  EntityId m_player{};

  Game();
  ~Game();

  void update(UserCommand& userCmd, Renderer& r);
  void updateUserCmd(UserCommand& userCmd, SDL_Scancode keyCode, bool isDown);

  EntityId createEntity(EntityType type, float x, float y);
  Entity* getEntity(EntityId e);
  void removeEntity(EntityId e);
};

#endif
