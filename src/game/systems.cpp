#include "systems.hpp"
#include "map.hpp"

bool collidesWithEntity(Game& game, Entity& e)
{
  bool collides = false;
  for (auto& other: game.m_entityPool)
  {
    if (other.id == e.id) continue;

    if (
      e.left() < other.right() &&
      e.right() > other.left() &&
      e.bottom() < other.top() &&
      e.top() > other.bottom())
    {
      collides = true;
    }
  }
  return collides;
}

void updatePhysics(Game& game)
{
  for (auto& e: game.m_entityPool)
  {
    if (e.vx == 0 && e.vy == 0) continue;

    float prevX = e.x; 
    e.x += e.vx;

    if (isColliding(game.m_loadedMap.ground, e) || collidesWithEntity(game, e))
    {
      e.x = prevX;
    }

    float prevY = e.y; 
    e.y += e.vy;
    if (isColliding(game.m_loadedMap.ground, e) || collidesWithEntity(game, e))
    {
      e.y = prevY;
    }

    e.vx = 0;
    e.vy = 0;
  }
}

void updateEntityInteraction(Entity& e, Game& game, UserCommand& userCmd)
{
  if (!e.interactable || !userCmd.activate) return;

  Entity* p = game.m_entityPool.getEntity(game.m_player);
  if (!p) return;

  float playerCenterX = p->x + p->width / 2;
  float playerCenterY = p->y + p->height / 2;

  constexpr int INTERACTION_RANGE = 10;

  if (
    ((playerCenterX >= e.left() && playerCenterX <= e.right())  &&
    ((p->direction == Direction::Up && e.bottom() - p->top() <= INTERACTION_RANGE && e.bottom() - p->top() >= -e.height) || 
      (p->direction == Direction::Down && p->bottom() - e.top() <= INTERACTION_RANGE && p->bottom() - e.top() >= -e.height))) ||
    ((playerCenterY >= e.bottom() && playerCenterY <= e.top())  &&
    ((p->direction == Direction::Right && e.left() - p->right() <= INTERACTION_RANGE && e.left() - p->right() >= -e.width) ||
      (p->direction == Direction::Left && p->left() - e.right() <= INTERACTION_RANGE && p->x - e.right() >= -e.width))))
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
