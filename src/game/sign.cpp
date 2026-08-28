#include "entities.hpp"
#include "game.hpp"

void onInteract(Game& game)
{
  game.playDialogue("[IMPORTANT NOTICE] test test t est tes ting 123 ");
}

void setupSign(Entity& e)
{
  e.sprite = {{64, 64}, {16, 16}};
  e.colliderHeightReduction = 0.5f;
  e.interactable = true;
  e.onInteract = onInteract;
  e.solid = true;
}
