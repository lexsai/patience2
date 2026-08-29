#include "entities.hpp"
#include "game.hpp"

#include <glm/glm.hpp>

std::map<std::string, std::vector<Frame>> playerAnimations 
{
  {
    "player_down", 
    {
      {{{0, 72}, {16, 24}}, 0.1f}, 
      {{{32, 72}, {16, 24}}, 0.1f},
      {{{0, 72}, {16, 24}}, 0.1f},
      {{{16, 72}, {16, 24}}, 0.1f}
    }
  },
  {
    "player_right", 
    {
      {{{0, 48}, {16, 24}}, 0.1f},
      {{{16, 48}, {16, 24}}, 0.1f},
      {{{0, 48}, {16, 24}}, 0.1f},
      {{{32, 48}, {16, 24}}, 0.1f}
    }
  },
  {
    "player_left", 
    {
      {{{0, 24}, {16, 24}}, 0.1f},
      {{{16, 24}, {16, 24}}, 0.1f},
      {{{0, 24}, {16, 24}}, 0.1f},
      {{{32, 24}, {16, 24}}, 0.1f}
    }
  },
  {
    "player_up",
    {
      {{{0, 0}, {16, 24}}, 0.1f},
      {{{16, 0}, {16, 24}}, 0.1f},
      {{{0, 0}, {16, 24}}, 0.1f},
      {{{32, 0}, {16, 24}}, 0.1f}
    }
  },
  {
    "player_down_idle",
    {
      {{{0, 72}, {16, 24}}, 0.1f}
    }
  },
  {
    "player_right_idle", 
    {
      {{{0, 48}, {16, 24}}, 0.1f}
    }
  },
  {
    "player_left_idle", 
    {
      {{{0, 24}, {16, 24}}, 0.1f}
    }
  },
  {
    "player_up_idle", 
    {
      {{{0, 0}, {16, 24}}, 0.1f}
    }
  },
};

void setupPlayer(Entity& e)
{
  e.width = TILE_WIDTH;
  e.height = TILE_WIDTH * 1.5f;
  e.hasAnimation = true;
  e.currentAnimation = "player_down_idle";
}

void updatePlayer(UserCommand& userCmd, Entity& e)
{
  glm::vec2 movement{};
  if (userCmd.forward)
  {
    movement.y = 1.0f;
  }
  if (userCmd.back)
  {
    movement.y = -1.0f;
  }
  if (userCmd.left)
  {
    movement.x = -1.0f;
  }
  if (userCmd.right)
  {
    movement.x = 1.0f;
  }

  if (movement.x > 0)
  {
    e.direction = Direction::Right;
  }
  else if (movement.x < 0) 
  {
    e.direction = Direction::Left;
  } 
  else if (movement.y > 0)
  {
    e.direction = Direction::Up;
  }
  else if (movement.y < 0)
  {
    e.direction = Direction::Down;
  }

  if (movement.x != 0 || movement.y != 0)
  {
    movement = glm::normalize(movement) * 4.0f;
  }

  e.vx = movement.x;
  e.vy = movement.y;

  std::string animation = "player_";
  switch (e.direction)
  {
    case Direction::Up:
      animation += "up";
      break;
    case Direction::Left:
      animation += "left";
      break;
    case Direction::Right:
      animation += "right";
      break;
    case Direction::Down:
      animation += "down";
      break;
  }
  if (!(userCmd.forward || userCmd.back || userCmd.left || userCmd.right))
  {
    animation += "_idle";
  }
  if (animation != e.currentAnimation)
  {
    e.currentFrameIndex = 0;
    e.currentAnimation = animation;
  }
}

void setupPlayerAnimations(Game& game)
{
  game.animations.insert(playerAnimations.begin(), playerAnimations.end());
}
