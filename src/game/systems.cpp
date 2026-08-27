#include "systems.hpp"

void updatePhysics(Game& game)
{
  for (auto& e: game.m_entities)
  {
    if (!game.m_allocated[e.id.index]) continue;

    e.x += e.vx;
    e.y += e.vy;

    e.vx = 0;
    e.vy = 0;
  }
}

void updateEntityInteraction(Entity& e, Game& game, UserCommand& userCmd)
{
  if (!e.interactable || !userCmd.activate)
  {
    return;
  }

  Entity* p = game.getEntity(game.m_player);
  if (!p) 
  {
    return;
  }

  float playerCenterX = p->x + 24;
  float playerCenterY = p->y + 36;

  float playerTop = p->y + 72;
  float playerRight = p->x + 48;

  float entityTop = e.y + 48;
  float entityRight = e.x + 48;

  if (
    ((playerCenterX >= e.x && playerCenterX <= e.x + 48)  &&
    ((p->direction == Direction::Up && e.y - playerTop <= 10 && e.y - playerTop >= -32) || 
      (p->direction == Direction::Down && p->y - entityTop <= 10 && p->y - entityTop >= -32))) ||
    ((playerCenterY >= e.y && playerCenterY <= e.y + 48)  &&
    ((p->direction == Direction::Right && e.x - playerRight <= 10 && e.x - playerRight >= -32) ||
      (p->direction == Direction::Left && p->x - entityRight <= 10 && p->x - entityRight >= -32))))
  {
    SDL_Log("interacted!");
    // TODO: better way to prevent activate from insta closing the dialogue.
    userCmd.activate = false;
    game.playDialogue("black stars hang in the heavens.");
  }
}

void updateEntityAnimation(Entity& e)
{
  if (!e.hasAnimation)
  {
    return;
  }

  std::vector<Frame> frames = e.anim.animations[e.anim.currentAnimation];
  Frame currentFrame = frames[e.anim.currentFrame];
  
  e.sprite = currentFrame.sprite;
  
  e.anim.animationTime += 0.015f;
  if (e.anim.animationTime > currentFrame.duration) {
    e.anim.animationTime = 0.0f;
    if (e.anim.currentFrame + 1 == frames.size()) {
      e.anim.currentFrame = 0;
    } else {
      e.anim.currentFrame += 1;
    }
  }
}
