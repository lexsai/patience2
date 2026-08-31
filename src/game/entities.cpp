#include "entities.hpp"
#include "game.hpp"

// NOTE: keep all switches on EntityType in this file so we 
//       dont lose track of them.
void updateEntity(Entity& e, UserCommand& userCmd)
{
  switch (e.type)
  {
    case EntityType::Player:
      updatePlayer(userCmd, e);
      break;
    default:
      break;
  }
}

void updateBattleState(EntityType type, BattleState& bs)
{
  switch (type)
  {
    case EntityType::Slime:
      updateSlimeBattle(bs);
      break;
  }
}

void setupEntity(Entity& e)
{
  switch (e.type)
  {
    case EntityType::Player:
      setupPlayer(e);
      break;
    case EntityType::Sign:
      setupSign(e);
      break;
    case EntityType::Slime:
      setupSlime(e);
      break;
  }
}


