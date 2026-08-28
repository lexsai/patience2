#include "game.hpp"
#include "systems.hpp"

#include <stdexcept>
#include <cstring>

Game::Game(Renderer& r)
{
  m_player = m_entityPool.createEntity(EntityType::Player, 300, 300);
  m_entityPool.createEntity(EntityType::Sign, 300, 320);

  EntityId m_test = m_entityPool.createEntity(EntityType::Sign, 360, 320);
  m_entityPool.createEntity(EntityType::Sign, 200, 320);
  m_entityPool.createEntity(EntityType::Sign, 100, 320);
  m_entityPool.createEntity(EntityType::Sign, 100, 200);
  m_entityPool.createEntity(EntityType::Sign, 100, 100);
  m_entityPool.removeEntity(m_test);

  setupPlayerAnimations(*this);

  m_loadedMap = loadMap("assets/map.txt");
  for (int y = 0; y < m_loadedMap.ground.height; y++)
  {
    for (int x = 0; x< m_loadedMap.ground.width; x++)
    {
      Tile tile = m_loadedMap.ground.tiles[y][x];
      r.drawStaticSprite(
        tile.sprite, 
        static_cast<float>(x) * TILE_WIDTH, 
        static_cast<float>(y) * TILE_WIDTH, 
        TILE_WIDTH, TILE_WIDTH);
    }
  }
}

Game::~Game()
{
}

void Game::update(UserCommand& userCmd, Renderer& r)
{
  // if (userCmd.activate)
  // {
  //   m_entityPool.createEntity(EntityType::Player, 160, 160);
  // }

  if (!m_inDialogue)
  {
    // freeze game when in dialogue for now
    updateEntityLogic(userCmd);
    updateSystems();
  }

  updateHUD(userCmd);

  drawEntities(r);
  drawCeiling(r);
  drawHUD(r);

  resetUserCmd(userCmd);
}

void Game::updateEntityLogic(UserCommand& userCmd)
{
  // SDL_Log("test");
  for (auto& e: m_entityPool) 
  {
    // SDL_Log("e %d", e.id.index);
    updateEntity(e, userCmd);

    updateEntityAnimation(e, *this);
    updateEntityInteraction(e, *this, userCmd);
  }
}

void Game::updateSystems()
{
  updatePhysics(*this);
}

void Game::drawEntities(Renderer& r)
{
  for (auto& e: m_entityPool) 
  {
    r.drawSprite(e.sprite, e.x, e.y, e.width, 0);

    if (m_debugMode)
    {
      r.drawSprite({{0,0}, {16, 16}}, e.x, e.y, 16, 0);
    }
  }
}

void Game::drawCeiling(Renderer& r)
{
  for (int y = 0; y < m_loadedMap.ceiling.height; y++)
  {
    for (int x = 0; x< m_loadedMap.ceiling.width; x++)
    {
      Tile tile = m_loadedMap.ceiling.tiles[y][x];
      r.drawSprite(
        tile.sprite, 
        static_cast<float>(x) * TILE_WIDTH, 
        static_cast<float>(y) * TILE_WIDTH, 
        TILE_WIDTH, TILE_WIDTH);
    }
  }
}

void Game::drawHUD(Renderer& r)
{
  if (m_debugMode)
  {
    int counter = 0;
    for (auto& e: m_entityPool)
    {
      r.drawText(
        "X: " + std::to_string(e.x) + " | Y: " + std::to_string(e.y) + " | Generation: " + std::to_string(e.id.generation) + " | index: " + std::to_string(e.id.index), 
        0, 440.0f - 40 * counter
      );
      counter++;
    }
  }

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
  if (userCmd.toggleDebug)
  {
    m_debugMode = !m_debugMode;
  }

  if (m_dialogueProgress < m_dialogue.length())
  {
    m_dialogueProgress++;
  }

  if (userCmd.activate && m_dialogueProgress != m_dialogue.length())
  {
    m_dialogueProgress = m_dialogue.length();
  }
  else if (userCmd.activate && m_dialogueProgress == m_dialogue.length())
  {
    m_dialogue = "";
    m_dialogueProgress = 0;
    m_inDialogue = false;
  }
}

void Game::resetUserCmd(UserCommand& userCmd)
{
  userCmd.activate = false;
  userCmd.toggleDebug = false;
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
      if (isDown) userCmd.activate = true;
      break;

    case SDL_SCANCODE_U:
      if (isDown) userCmd.toggleDebug = true;
      break;
  }
}

void Game::playDialogue(std::string text)
{
  m_inDialogue = true;
  m_dialogue = text;
  m_dialogueProgress = 0;
}
