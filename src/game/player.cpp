#include "entities.hpp"
#include "game.hpp"

std::map<std::string, std::vector<Frame>> playerAnimations 
{
  {
    "down", 
    {
      {{{0, 72}, {16, 24}}, 0.25f}, 
      {{{32, 72}, {16, 24}}, 0.25f},
      {{{0, 72}, {16, 24}}, 0.25f},
      {{{16, 72}, {16, 24}}, 0.25f}
    }
  },
  {
    "right", 
    {
      {{{0, 48}, {16, 24}}, 0.25f},
      {{{16, 48}, {16, 24}}, 0.25f},
      {{{0, 48}, {16, 24}}, 0.25f},
      {{{32, 48}, {16, 24}}, 0.25f}
    }
  },
  {
    "left", 
    {
      {{{0, 24}, {16, 24}}, 0.25f},
      {{{16, 24}, {16, 24}}, 0.25f},
      {{{0, 24}, {16, 24}}, 0.25f},
      {{{32, 24}, {16, 24}}, 0.25f}
    }
  },
  {
    "up",
    {
      {{{0, 0}, {16, 24}}, 0.25f},
      {{{16, 0}, {16, 24}}, 0.25f},
      {{{0, 0}, {16, 24}}, 0.25f},
      {{{32, 0}, {16, 24}}, 0.25f}
    }
  },
  {
    "down_idle",
    {
      {{{0, 72}, {16, 24}}, 0.25f}
    }
  },
  {
    "right_idle", 
    {
      {{{0, 48}, {16, 24}}, 0.25f}
    }
  },
  {
    "left_idle", 
    {
      {{{0, 24}, {16, 24}}, 0.25f}
    }
  },
  {
    "up_idle", 
    {
      {{{0, 0}, {16, 24}}, 0.25f}
    }
  },
};

void setupPlayer(Entity& e)
{
  e.hasAnimation = true;
  e.anim.animations = playerAnimations;
  e.anim.currentAnimation = "down_idle";
}

void updatePlayer(UserCommand& userCmd, Entity& e)
{
  if (userCmd.forward)
  {
    e.vy = 5.0f;
  }
  if (userCmd.back)
  {
    e.vy = -5.0f;
  }
  if (userCmd.left)
  {
    e.vx = -5.0f;
  }
  if (userCmd.right)
  {
    e.vx = 5.0f;
  }

  if (e.vx > 0)
  {
    e.direction = Direction::Right;
  }
  else if (e.vx < 0) 
  {
    e.direction = Direction::Left;
  } 
  else if (e.vy > 0)
  {
    e.direction = Direction::Up;
  }
  else if (e.vy < 0)
  {
    e.direction = Direction::Down;
  }

  std::string animation = "";
  switch (e.direction)
  {
    case Direction::Up:
      animation = "up";
      break;
    case Direction::Left:
      animation = "left";
      break;
    case Direction::Right:
      animation = "right";
      break;
    case Direction::Down:
      animation = "down";
      break;
  }
  if (!(userCmd.forward || userCmd.back || userCmd.left || userCmd.right))
  {
    animation += "_idle";
  }
  if (animation != e.anim.currentAnimation)
  {
    e.anim.currentFrame = 0;
    e.anim.currentAnimation = animation;
  }
}
