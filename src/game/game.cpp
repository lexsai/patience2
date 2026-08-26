#include "game.hpp"

Game::Game()
{
  m_player = createEntity(EntityType::Player, 160, 160);
}

Game::~Game()
{

}

void updatePlayer(UserCommand& userCmd, Entity& player)
{
  if (userCmd.forward) {
    player.vy = 5.0f;
  }
  if (userCmd.back) {
    player.vy = -5.0f;
  }
  if (userCmd.left) {
    player.vx = -5.0f;
  }
  if (userCmd.right) {
    player.vx = 5.0f;
  }
}

void Game::updateEntityLogic(UserCommand& userCmd)
{
  for (auto& entity: m_entities) 
  {
    switch (entity.type)
    {
      case EntityType::Player:
        updatePlayer(userCmd, entity);
        break;
      default:
        break;
    }
  }
}

void updatePhysics(Game& game)
{
  for (auto& entity: game.m_entities)
  {
    entity.x += entity.vx;
    entity.y += entity.vy;

    entity.vx = 0;
    entity.vy = 0;
  }
}

void Game::updateSystems()
{
  updatePhysics(*this);
}

void Game::drawEntities(Renderer& r)
{
  for (const auto& entity: m_entities) 
  {
    r.drawSprite({{0, 72}, {16, 24}}, entity.x, entity.y, 48, 0);
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
      "X: " + std::to_string(player->x) + " | Y: " + std::to_string(player->y) + " | Generation: " + std::to_string(player->entityId.generation), 
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
  int generation = m_nextGeneration++;
  Entity entity{ { generation, m_entities.size() }, type, x, y };
  m_entities.push_back(entity);

  return entity.entityId;
}

Entity* Game::getEntity(EntityId e)
{
  Entity* entity = &m_entities[e.index];
  if (entity->entityId.generation != e.generation)
  {
    return nullptr;
  }

  return entity;
}

void Game::removeEntity(EntityId e)
{
  m_entities.erase(m_entities.begin() + e.index);
}
