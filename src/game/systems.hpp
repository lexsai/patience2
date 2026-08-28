#ifndef PATIENCE_SYSTEMS_HPP
#define PATIENCE_SYSTEMS_HPP

#include "game.hpp"

void updatePhysics(Game& game);
void updateEntityAnimation(Entity& e, Game& game);
void updateEntityInteraction(Entity& e, Game& game, UserCommand& userCmd);

#endif
