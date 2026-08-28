#ifndef PATIENCE_GAME_HPP
#define PATIENCE_GAME_HPP

#include <array>

#include "../renderer/renderer.hpp"
#include "entities.hpp"
#include "entity_pool.hpp"
#include "map.hpp"

struct UserCommand {
  bool forward;
  bool back;
  bool left;
  bool right;
  bool activate;
  bool toggleDebug;
};

class Game
{
  bool m_debugMode{};

  bool m_inDialogue{};
  std::string m_dialogue{};
  size_t m_dialogueProgress{};

  void updateEntityLogic(UserCommand& userCmd);
  void updateSystems();

  void updateHUD(UserCommand& userCmd);

  void drawEntities(Renderer& r);
  void drawCeiling(Renderer& r);
  void drawHUD(Renderer& r);
public:
  EntityPool m_entityPool{};

  std::map<std::string, std::vector<Frame>> animations;

  EntityId m_player{};

  Map m_loadedMap{};

  Game(Renderer& renderer);
  ~Game();

  void update(UserCommand& userCmd, Renderer& r);
  void updateUserCmd(UserCommand& userCmd, SDL_Scancode keyCode, bool isDown);
  void resetUserCmd(UserCommand& userCmd);

  void playDialogue(std::string text);
};

#endif
