#include "game.hpp"
#include "systems.hpp"
#include "game_hud.hpp"

#include <SDL3/SDL.h>

#include <stdexcept>
#include <cstring>

Game::Game(Renderer& r)
  : m_windowWidth{ r.m_windowWidth }, m_windowHeight{ r.m_windowHeight }
{
  m_player = m_entityPool.createEntity(EntityType::Player, 300, 100);
  m_entityPool.createEntity(EntityType::Sign, 300, 320);

  EntityId m_test = m_entityPool.createEntity(EntityType::Sign, 360, 320);
  m_entityPool.createEntity(EntityType::Sign, 200, 320);
  m_entityPool.createEntity(EntityType::Sign, 100, 320);
  m_entityPool.createEntity(EntityType::Sign, 100, 200);
  m_entityPool.createEntity(EntityType::Sign, 100, 100);
  m_entityPool.removeEntity(m_test);

  m_entityPool.createEntity(EntityType::Slime, 400, 100);

  setupPlayerAnimations(*this);

  m_loadedMap = loadMap("assets/map.txt");
  // for (int y = 0; y < m_loadedMap.ground.height; y++)
  // {
  //   for (int x = 0; x< m_loadedMap.ground.width; x++)
  //   {
  //     Tile tile = m_loadedMap.ground.tiles[y][x];
  //     r.drawStaticSprite(
  //       tile.sprite, 
  //       static_cast<float>(x) * TILE_WIDTH, 
  //       static_cast<float>(y) * TILE_WIDTH, 
  //       TILE_WIDTH, TILE_WIDTH);
  //   }
  // }
}

Game::~Game()
{
}

bool Game::isOnScreen(float wX, float wY)
{
  Entity* p = m_entityPool.getEntity(m_player);
  return abs(p->x - wX) <= m_windowWidth / 2.0f + TILE_WIDTH && 
    abs(p->y - wY) <= m_windowHeight / 2.0f + TILE_WIDTH;
}

void Game::update(UserCommand& userCmd, Renderer& r)
{
  if (m_battleState.enemy != EntityType::Nil)
  {
    updateBattleState(m_battleState.enemy, m_battleState);
    drawBattle(r);
    resetUserCmd(userCmd);
    return;
  }

  if (m_dialogue.empty())
  {
    // freeze game when in dialogue for now
    updateEntityLogic(userCmd);
    updateSystems();
  }

  updateCamera(r);
  updateHUD(userCmd);

  drawWorld(r, userCmd);

  resetUserCmd(userCmd);
}

void Game::updateCamera(Renderer& r)
{
  Entity* player = m_entityPool.getEntity(m_player);
  r.m_camera = { player->x, player->y };
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

void Game::drawWorld(Renderer& r, UserCommand& userCmd)
{
  Entity* player = m_entityPool.getEntity(m_player);
  for (int y = 0; y < m_loadedMap.ground.height; y++)
  {
    for (int x = 0; x< m_loadedMap.ground.width; x++)
    {
      Tile tile = m_loadedMap.ground.tiles[y][x];
      float worldX = static_cast<float>(x) * TILE_WIDTH;
      float worldY = static_cast<float>(y) * TILE_WIDTH;

      if (isOnScreen(worldX, worldY))
      {
        r.drawSprite(
          tile.sprite, 
          worldX, worldY, 
          TILE_WIDTH, TILE_WIDTH);
      }
    }
  }
  drawEntities(r);
  drawCeiling(r);

  drawHUD(r, userCmd);
}

void Game::drawBattle(Renderer& r)
{
  r.drawText("IN BATTLE", 640, 480);
  r.drawHudSprite({{0, 48}, {16, 24}}, 300, 400, 100, 0);
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

void Game::drawHUD(Renderer& r, UserCommand& userCmd)
{
  if (m_editMode)
  {
    hudBegin();
    hudBeginContainer(r, userCmd, 100.0f, 600.0f);
      hudLabel(
        r, userCmd, 
        "selectedTile: " + std::to_string(m_selectedTileId), 
        0.0f, 0.0f);
      if (hudButton(r, userCmd, "empty", 0.0f, 0.0f))
      {
        m_selectedTileId = 0;
      }
      if (hudButton(r, userCmd, "tile 1", 0.0f, 0.0f))
      {
        m_selectedTileId = 1;
      }
      if (hudButton(r, userCmd, "tile 2", 0.0f, 0.0f))
      {
        m_selectedTileId = 2;
      }
      if (hudButton(r, userCmd, "export map", 0.0f, 0.0f))
      {
        exportMap(m_loadedMap, "assets/map.txt");
      }
    hudEndContainer(r, userCmd);

    glm::vec2 mouseTile = worldPosToTilePos(
      userCmd.worldMouseX, userCmd.worldMouseY);
    r.drawSprite({{48, 0}, {16, 16}}, 
      mouseTile.x, mouseTile.y, 
      TILE_WIDTH, TILE_WIDTH);

    if (userCmd.click)
    { 
      Tile* tile = m_loadedMap.ground.get(
        userCmd.worldMouseX, userCmd.worldMouseY);
      *tile = tileById[m_selectedTileId];
    }
  }

  if (m_debugMode)
  {
    r.drawText(
      "X: " + std::to_string(userCmd.mouseX) + " | Y: " + std::to_string(userCmd.mouseY), 
      320, 100.0f
    );
    r.drawText(
      "aX: " + std::to_string(userCmd.worldMouseX) + " | aY: " + std::to_string(userCmd.worldMouseY), 
      320, 200.0f
    );

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

  if (!m_dialogue.empty())
  {
    r.drawHudSprite({{0, 96}, {640, 120}}, 344, 200, 620, 150);
    r.drawText(
      m_dialogue.substr(0, m_dialogueProgress), 
      380, 300
    );
  }
}

void Game::updateHUD(UserCommand& userCmd)
{
  if (userCmd.toggleDebug)
  {
    m_debugMode = !m_debugMode;
  }

  if (userCmd.toggleEdit)
  {
    m_editMode = !m_editMode;
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
  }
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

    case SDL_SCANCODE_E:
      if (isDown) userCmd.toggleEdit = true;
      break;
  }
}

void Game::resetUserCmd(UserCommand& userCmd)
{
  userCmd.activate = false;
  userCmd.toggleDebug = false;
  userCmd.toggleEdit = false;
  userCmd.clickOnce = false;
}

void Game::updateUserCmdClick(
  UserCommand& userCmd, bool isLeft, bool isDown)
{
  if (isLeft && isDown)
  {
    userCmd.clickOnce = true;
  }

  if (isLeft)
  {
    userCmd.click = isDown;
  }
}

void Game::updateUserCmdMousePos(
  UserCommand& userCmd, float mouseX, float mouseY)
{
  Entity *p = m_entityPool.getEntity(m_player);
  userCmd.mouseX = mouseX;
  userCmd.mouseY = m_windowHeight - mouseY;
  userCmd.worldMouseX = p->x - m_windowWidth / 2 + mouseX;  
  userCmd.worldMouseY = p->y + m_windowHeight / 2 - mouseY;
}

void Game::playDialogue(std::string text)
{
  m_dialogue = text;
  m_dialogueProgress = 0;
}

void Game::startBattle(BattleState bs)
{
  m_battleState = bs;
}
