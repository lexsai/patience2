#include "game.hpp"
#include "systems.hpp"

Game::Game()
{
  m_player = createEntity(EntityType::Player, 160, 160);
}

Game::~Game()
{
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
  }
}

void Game::updateSystems()
{
  updatePhysics(*this);
  updateAnimations(*this);
}

void Game::drawEntities(Renderer& r)
{
  for (const auto& e: m_entities) 
  {
    // TODO: consider missing texture case. what do ? 
    r.drawSprite(e.currentSprite, e.x, e.y, 48, 0);
  }
}

void Game::drawHUD(Renderer& r)
{
  r.drawText(
    std::string("the black stars which hang in the sky over Carcosa."), 
    0, 160
  );
  r.drawSprite({{0, 72}, {16, 24}}, 0, 200, 48, 0);

  Entity* player = getEntity(m_player);
  if (player)
  {
    r.drawText(
      "X: " + std::to_string(player->x) + " | Y: " + std::to_string(player->y) + " | Generation: " + std::to_string(player->id.generation), 
      0, 400
    );
  }
}

void Game::update(UserCommand& userCmd, Renderer& r)
{
  if (userCmd.test)
  {
    removeEntity(m_player);
    m_player = createEntity(EntityType::Player, 160, 160);
  }
  userCmd.test = false;

  updateEntityLogic(userCmd);
  updateSystems();

  drawHUD(r);
  drawEntities(r);
}

void Game::updateUserCmd(UserCommand& userCmd, SDL_Scancode keyCode, bool isDown)
{
  switch (keyCode) {
    case SDL_SCANCODE_W: {
      userCmd.forward = isDown;
    } break;

    case SDL_SCANCODE_S: {
      userCmd.back = isDown;
    } break;
    
    case SDL_SCANCODE_A: {
      userCmd.left = isDown;
    } break;

    case SDL_SCANCODE_D: {
      userCmd.right = isDown;
    } break;

    case SDL_SCANCODE_R: {
      if (!isDown)
      {
        userCmd.test = true;
      }
    } break;
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
