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
  bool activate;
};

class Game
{
  int m_nextGeneration{};

  bool m_inDialogue{};
  std::string m_dialogue{};
  size_t m_dialogueProgress{};

  void updateEntityLogic(UserCommand& userCmd);
  void updateSystems();

  void updateHUD(UserCommand& userCmd);

  void drawEntities(Renderer& r);
  void drawHUD(Renderer& r);
public:
  std::vector<Entity> m_entities{};
  EntityId m_player{};

  Game();
  ~Game();

  void update(UserCommand& userCmd, Renderer& r);
  void updateUserCmd(UserCommand& userCmd, SDL_Scancode keyCode, bool isDown);
  void resetUserCmd(UserCommand& userCmd);

  EntityId createEntity(EntityType type, float x, float y);
  Entity* getEntity(EntityId e);
  void removeEntity(EntityId e);

  void playDialogue(std::string text);
};

#endif
