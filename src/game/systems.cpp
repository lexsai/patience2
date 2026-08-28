#include "systems.hpp"

void updatePhysics(Game& game)
{
  for (auto& e: game.m_entityPool)
  {
    e.x += e.vx;
    e.y += e.vy;

    e.vx = 0;
    e.vy = 0;
  }
}

void updateEntityInteraction(Entity& e, Game& game, UserCommand& userCmd)
{
  if (!e.interactable || !userCmd.activate) return;

  Entity* p = game.m_entityPool.getEntity(game.m_player);
  if (!p) return;

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
    // TODO: better way to prevent activate from insta closing the dialogue.
    userCmd.activate = false;
    if (e.onInteract)
    {
      e.onInteract(game);
    }
  }
}

void updateEntityAnimation(Entity& e, Game& game)
{
  if (!e.hasAnimation) return;

  std::vector<Frame> frames = game.animations[e.currentAnimation];
  Frame currentFrame = frames[e.currentFrameIndex];
  
  e.sprite = currentFrame.sprite;
  
  e.animationTime += 0.015f;
  if (e.animationTime > currentFrame.duration) {
    e.animationTime = 0.0f;
    if (e.currentFrameIndex + 1 == frames.size()) {
      e.currentFrameIndex = 0;
    } else {
      e.currentFrameIndex += 1;
    }
  }
}
