#ifndef PATIENCE_ENTITIES_HPP
#define PATIENCE_ENTITIES_HPP

#include "../renderer/renderer.hpp"

struct UserCommand;

struct EntityId
{
  int generation;
  size_t index;
};

enum class EntityType
{
  None, // reserved for invalid entity
  Player,
};

enum class Direction
{
  Up,
  Down,
  Left,
  Right
};

struct Frame
{
  SpriteAtlasSpecifier sprite;
  float duration;
};

struct Animation
{
  float animationTime;
  int currentFrame;
  std::map<std::string, std::vector<Frame>> animations;
  std::string currentAnimation;
};

struct Entity
{
  EntityId id;

  EntityType type;
  float x;
  float y;

  float vx;
  float vy;

  SpriteAtlasSpecifier currentSprite;
  Direction direction;
  Animation anim;
};

void setupPlayer(Entity& e);
void updatePlayer(UserCommand& userCmd, Entity& e);

#endif
