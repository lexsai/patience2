#ifndef PATIENCE_ENTITIES_HPP
#define PATIENCE_ENTITIES_HPP

#include "../renderer/renderer.hpp"

struct UserCommand;
class Game;

struct EntityId
{
  int generation;
  int index;

  bool operator==(EntityId& other)
  {
    return index == other.index && generation == other.generation;
  }
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

  float width;
  float height;

  SpriteAtlasSpecifier sprite;

  Direction direction;

  bool solid;
  // kind of wack but fits with zii so
  float colliderHeightReduction;

  bool hasAnimation;
  float animationTime;
  int currentFrameIndex;

  // TODO: change to id reference in manager, bad to have heap alloc & ctor 
  //        overhead for all entities
  std::string currentAnimation;
  
  bool interactable;
  void (*onInteract)(Game&);

  explicit operator bool() const
  {
    return type != EntityType::Nil;
  }
 
  // for collisions
  float left() { return x; }
  float right() { return x + width; }
  float top() { return y + height * (1 - colliderHeightReduction); }
  float bottom() { return y; }
};

void updateEntity(Entity& e, UserCommand& userCmd);
void setupEntity(Entity& e);

void setupPlayer(Entity& e);
void updatePlayer(UserCommand& userCmd, Entity& e);
void setupPlayerAnimations(Game& game);

void setupSign(Entity& e);

#endif
