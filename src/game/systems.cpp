#include "systems.hpp"

void updatePhysics(Game& game)
{
  for (auto& e: game.m_entities)
  {
    e.x += e.vx;
    e.y += e.vy;

    e.vx = 0;
    e.vy = 0;
  }
}

void updateAnimations(Game& game)
{
  for (auto& e: game.m_entities)
  {
    std::vector<Frame> frames = e.anim.animations[e.anim.currentAnimation];
    Frame currentFrame = frames[e.anim.currentFrame];
    
    e.currentSprite = currentFrame.sprite;
    
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
}
