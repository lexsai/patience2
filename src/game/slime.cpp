#include "entities.hpp"
#include "game.hpp"

namespace
{
  void onInteract(Game& game)
  {
    BattleState bs;
    bs.hp = 100.0f;
    bs.enemyHP = 100.0f;
    bs.enemy = EntityType::Slime;
    game.startBattle(bs);
  }
}

void setupSlime(Entity& e)
{
  e.sprite = {{64, 16}, {16, 16}};
  e.interactable = true;
  e.onInteract = onInteract;
  e.solid = true;
}

void updateSlimeBattle(BattleState& bs)
{
  // TODO todo
}
