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
  bool toggleEdit;
  bool clickOnce;
  bool click;
  float mouseX;
  float mouseY;
  float worldMouseX;
  float worldMouseY;
};

class Game
{
  bool m_debugMode{};

  bool m_editMode{};
  int m_selectedTileId{};

  bool m_inDialogue{};
  std::string m_dialogue{};
  size_t m_dialogueProgress{};

  void updateEntityLogic(UserCommand& userCmd);
  void updateSystems();
  void updateCamera(Renderer& r);

  void updateHUD(UserCommand& userCmd);

  void drawEntities(Renderer& r);
  void drawCeiling(Renderer& r);
  void drawHUD(Renderer& r, UserCommand& userCmd);
public:
  EntityPool m_entityPool{};

  std::map<std::string, std::vector<Frame>> animations;

  EntityId m_player{};

  Map m_loadedMap{};

  // yes we dupe this state but ill fix this when we add window resizing
  float m_windowWidth{};
  float m_windowHeight{};

  Game(Renderer& renderer);
  ~Game();

  void update(UserCommand& userCmd, Renderer& r);
  void updateUserCmd(
    UserCommand& userCmd, SDL_Scancode keyCode, bool isDown);
  void updateUserCmdClick(
    UserCommand& userCmd, bool isLeft, bool isDown);
  void updateUserCmdMousePos(
    UserCommand& userCmd, float mouseX, float mouseY);
  void resetUserCmd(UserCommand& userCmd);

  void playDialogue(std::string text);
};

#endif
