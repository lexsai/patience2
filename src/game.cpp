#include "game.hpp"

Game::Game()
{
}

Game::~Game()
{

}

void Game::update(UserCommand& userCmd, Renderer& r)
{
  r.drawText(
    std::string("the black stars which hang in the sky over Carcosa."), 
    0, 160
  );

  r.drawSprite({{0, 72}, {16, 24}}, 0, 160, 48, 0);
  // for (Entity entity: m_entities) 
  // {
  //   entity.update(*this, userCmd);
  // }

  // // draw
  // for (Entity entity: m_entities) 
  // {
  // }
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
  }
}

void Game::createEntity(float x, float y)
{
  Entity entity{ x, y };
  m_entities.push_back(entity);
}
