#include "game.hpp"
#include "systems.hpp"

#include <stdexcept>
#include <cstring>

Game::Game()
{
  m_freeListHead = 1;
  for (int i = 1; i < MAX_ENTITIES - 1; i++)
  {
    m_nextFreeSlot[i] = i + 1;
  }
  // reserve m_entities[0] as nil entity
  m_nextFreeSlot[MAX_ENTITIES - 1] = 0;

  m_player = createEntity(EntityType::Player, 160, 160);
  createEntity(EntityType::Sign, 320, 320);
  EntityId m_test = createEntity(EntityType::Sign, 360, 320);
  removeEntity(m_test);
  createEntity(EntityType::Sign, 100, 320);
}

Game::~Game()
{
}

void Game::update(UserCommand& userCmd, Renderer& r)
{
  // if (userCmd.activate)
  // {
  //   removeEntity(m_player);
  //   m_player = createEntity(EntityType::Player, 160, 160);
  // }

  if (!m_inDialogue)
  {
    // freeze game when in dialogue for now
    updateEntityLogic(userCmd);
    updateSystems();
  }

  updateHUD(userCmd);

  drawHUD(r);
  drawEntities(r);

  resetUserCmd(userCmd);
}

void Game::updateEntityLogic(UserCommand& userCmd)
{
  for (auto& e: m_entities) 
  {
    if (!m_allocated[e.id.index]) continue;

    switch (e.type)
    {
      case EntityType::Player:
        updatePlayer(userCmd, e);
        break;
      default:
        break;
    }

    updateEntityAnimation(e);
    updateEntityInteraction(e, *this, userCmd);
  }
}

void Game::updateSystems()
{
  updatePhysics(*this);
}

void Game::drawEntities(Renderer& r)
{
  for (const auto& e: m_entities) 
  {
    if (!m_allocated[e.id.index]) continue;
    r.drawSprite(e.sprite, e.x, e.y, 48, 0);
    r.drawSprite({{0,0}, {16, 16}}, e.x, e.y, 16, 0);
  }
}

void Game::drawHUD(Renderer& r)
{
  // r.drawText(
  //   std::string("the black stars which hang in the sky over Carcosa."), 
  //   0, 160
  // );
  // r.drawSprite({{0, 72}, {16, 24}}, 0, 200, 48, 0);

  // Entity* player = getEntity(m_player);
  // if (player)
  // {
  int counter = 0;
  for (auto& e: m_entities) 
  {
    if (!m_allocated[e.id.index]) continue;

    r.drawText(
      "X: " + std::to_string(e.x) + " | Y: " + std::to_string(e.y) + " | Generation: " + std::to_string(e.id.generation) + " | index: " + std::to_string(e.id.index), 
      0, 440.0f - 40 * counter
    );
    counter++;
  }

    // Entity* sign = &m_entities[1];
    // r.drawText(
    //   "X: " + std::to_string(sign->x) + " | Y: " + std::to_string(sign->y) + " | Generation: " + std::to_string(sign->id.generation) + " | index: " + std::to_string(sign->id.index), 
    //   0, 440
    // );
  // }

  if (m_inDialogue)
  {
    r.drawSprite({{0, 96}, {640, 120}}, 10, 10, 620, 150);
    r.drawText(
      m_dialogue.substr(0, m_dialogueProgress), 
      50, 100
    );
  }
}

void Game::updateHUD(UserCommand& userCmd)
{
  if (m_dialogueProgress < m_dialogue.length())
  {
    m_dialogueProgress++;
  }

  if (userCmd.activate && m_dialogueProgress != m_dialogue.length())
  {
    m_dialogueProgress = m_dialogue.length();
  }
  else if (userCmd.activate && m_dialogueProgress == m_dialogue.length())
  // if (userCmd.activate)
  {
    m_dialogue = "";
    m_dialogueProgress = 0;
    m_inDialogue = false;
  }
}

void Game::resetUserCmd(UserCommand& userCmd)
{
  userCmd.activate = false;
}

void Game::updateUserCmd(UserCommand& userCmd, SDL_Scancode keyCode, bool isDown)
{
  switch (keyCode) {
    case SDL_SCANCODE_W:
      userCmd.forward = isDown;
      break;

    case SDL_SCANCODE_S:
      userCmd.back = isDown;
      break;
    
    case SDL_SCANCODE_A:
      userCmd.left = isDown;
      break;

    case SDL_SCANCODE_D:
      userCmd.right = isDown;
      break;

    case SDL_SCANCODE_R:
      if (isDown)
      {
        userCmd.activate = true;
      }
      break;
  }
}

EntityId Game::createEntity(EntityType type, float x, float y)
{
  int slot = m_freeListHead;
  m_freeListHead = m_nextFreeSlot[slot];
  
  Entity *e = &m_entities.at(slot);
  if (m_allocated[slot])
  {
    throw std::runtime_error("tried creating on allocated slot" + std::to_string(slot));
  }
  // prevent state from prev allocation from contaminating
  *e = {};
  m_allocated[slot] = true;

  int generation = m_generation.at(slot)++;
  e->id = { generation, slot };
  e->type = type;
  e->x = x;
  e->y = y;

  switch (type)
  {
    case EntityType::Player:
      setupPlayer(*e);
      break;
    case EntityType::Sign:
      setupSign(*e);
      break;
  }

  return e->id;
}

Entity* Game::getEntity(EntityId e)
{
  Entity* entity = &m_entities[e.index];
  if (entity->id.generation != e.generation || !m_allocated[e.index])
  {
    return &m_entities[0];
  }

  return entity;
}

void Game::removeEntity(EntityId e)
{
  if (e.index <= 0 || e.index >= MAX_ENTITIES) return;

  Entity *entity = &m_entities[e.index];
  if (entity->id.generation == e.generation)
  {
    if (e.index != entity->id.index)
    {
      throw std::runtime_error("inconsistent entity id state");
    }

    m_allocated[e.index] = false;

    m_nextFreeSlot[e.index] = m_freeListHead;
    m_freeListHead = e.index;
  }
}

void Game::playDialogue(std::string text)
{
  m_inDialogue = true;
  m_dialogue = text;
  m_dialogueProgress = 0;
}
