#ifndef PATIENCE_GAME_HPP
#define PATIENCE_GAME_HPP

#include <array>

#include "../renderer/renderer.hpp"
#include "entities.hpp"

constexpr int MAX_ENTITIES = 1024;

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
  std::array<Entity, MAX_ENTITIES> m_entities{};
  
  // free list for m_entities 
  std::array<int, MAX_ENTITIES> m_nextFreeSlot{};
  int m_freeListHead{};
  
  // keep these separate so we can reset entity state with copy assignment
  // of default entity without messing up array state
  std::array<bool, MAX_ENTITIES> m_allocated{};
  std::array<int, MAX_ENTITIES> m_generation{};

  std::map<std::string, std::vector<Frame>> animations;

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
