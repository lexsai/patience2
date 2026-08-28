#ifndef PATIENCE_GAME_HPP
#define PATIENCE_GAME_HPP

#include <array>

#include "../renderer/renderer.hpp"
#include "entities.hpp"
#include "entity_pool.hpp"

struct UserCommand {
  bool forward;
  bool back;
  bool left;
  bool right;
  bool activate;
};

class Game
{
  bool m_inDialogue{};
  std::string m_dialogue{};
  size_t m_dialogueProgress{};

  void updateEntityLogic(UserCommand& userCmd);
  void updateSystems();

  void updateHUD(UserCommand& userCmd);

  void drawEntities(Renderer& r);
  void drawHUD(Renderer& r);
public:
  EntityPool m_entityPool{};

  std::map<std::string, std::vector<Frame>> animations;

  EntityId m_player{};

  Game();
  ~Game();

  void update(UserCommand& userCmd, Renderer& r);
  void updateUserCmd(UserCommand& userCmd, SDL_Scancode keyCode, bool isDown);
  void resetUserCmd(UserCommand& userCmd);

  void playDialogue(std::string text);
};

#endif
