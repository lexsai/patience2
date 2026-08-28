#include "game.hpp"
#include "systems.hpp"

#include <stdexcept>
#include <cstring>

Game::Game()
{
  m_player = m_entityPool.createEntity(EntityType::Player, 160, 160);
  m_entityPool.createEntity(EntityType::Sign, 320, 320);

  EntityId m_test = m_entityPool.createEntity(EntityType::Sign, 360, 320);
  m_entityPool.removeEntity(m_test);
  m_entityPool.createEntity(EntityType::Sign, 100, 320);

  setupPlayerAnimations(*this);
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
  for (auto& e: m_entityPool) 
  {
    switch (e.type)
    {
      case EntityType::Player:
        updatePlayer(userCmd, e);
        break;
      default:
        break;
    }

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
    r.drawSprite(e.sprite, e.x, e.y, 48, 0);
    r.drawSprite({{0,0}, {16, 16}}, e.x, e.y, 16, 0);
  }
}

void Game::drawHUD(Renderer& r)
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

void Game::playDialogue(std::string text)
{
  m_inDialogue = true;
  m_dialogue = text;
  m_dialogueProgress = 0;
}
