#include "entities.hpp"
#include "game.hpp"

void onInteract(Game& game)
{
  game.playDialogue("black stars hang in the heavens.");
}

void setupSign(Entity& e)
{
  e.sprite = {{64, 64}, {16, 16}};
  e.interactable = true;
  e.onInteract = onInteract;
  e.solid = true;
}
