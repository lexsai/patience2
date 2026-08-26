#include "game.hpp"
#include "systems.hpp"

Game::Game()
{
  m_player = createEntity(EntityType::Player, 160, 160);
  createEntity(EntityType::Sign, 320, 320);
}

Game::~Game()
{
}

void Game::update(UserCommand& userCmd, Renderer& r)
{
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
    if (e.sprite.size.x == 0 || e.sprite.size.y == 0)
    {
      continue;
    }
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

  Entity* player = getEntity(m_player);
  if (player)
  {
    r.drawText(
      "X: " + std::to_string(player->x) + " | Y: " + std::to_string(player->y) + " | Generation: " + std::to_string(player->id.generation), 
      0, 400
    );

    // Entity* sign = &m_entities[1];
    // r.drawText(
    //   "X: " + std::to_string(sign->x) + " | Y: " + std::to_string(sign->y) + " | Generation: " + std::to_string(sign->id.generation), 
    //   0, 440
    // );
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

EntityId Game::createEntity(EntityType type, float x, float y)
{
  Entity e{};
  e.id = { m_nextGeneration++, m_entities.size() };
  e.type = type;
  e.x = x;
  e.y = y;

  switch (type)
  {
    case EntityType::Player:
      setupPlayer(e);
      break;
    case EntityType::Sign:
      setupSign(e);
      break;
  }

  m_entities.push_back(e);
  return e.id;
}

Entity* Game::getEntity(EntityId e)
{
  Entity* entity = &m_entities[e.index];
  if (entity->id.generation != e.generation)
  {
    return nullptr;
  }

  return entity;
}

void Game::removeEntity(EntityId e)
{
  m_entities.erase(m_entities.begin() + e.index);
}

void Game::playDialogue(std::string text)
{
  m_inDialogue = true;
  m_dialogue = text;
  m_dialogueProgress = 0;
}
