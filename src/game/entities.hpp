#ifndef PATIENCE_ENTITIES_HPP
#define PATIENCE_ENTITIES_HPP

#include "../renderer/renderer.hpp"

struct UserCommand;

struct EntityId
{
  int generation;
  int index;
};

enum class EntityType
{
  Nil, 
  Player,
  Sign,
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

  // TODO: change to id reference in manager, bad to have heap alloc & ctor 
  //        overhead for all entities
  std::map<std::string, std::vector<Frame>> animations;
  std::string currentAnimation;
};

// inspired by Anton Mikhailov's Large Array of Things system
// https://www.youtube.com/watch?v=ShSGHb65f3M
struct Entity
{
  EntityId id;
  EntityType type;

  float x;
  float y;

  float vx;
  float vy;

  SpriteAtlasSpecifier sprite;

  Direction direction;

  bool hasAnimation;
  Animation anim;
  
  bool interactable;

  explicit operator bool() const
  {
    return type != EntityType::Nil;
  }
};

void setupPlayer(Entity& e);
void updatePlayer(UserCommand& userCmd, Entity& e);

void setupSign(Entity& e);

#endif
