#ifndef PATIENCE_GAME_HUD_HPP
#define PATIENCE_GAME_HUD_HPP

#include "../renderer/renderer.hpp"

struct UserCommand;

void hudBegin();
void hudBeginContainer(Renderer& r, UserCommand& userCmd, float x, float y);
void hudEndContainer(Renderer& r, UserCommand& userCmd);
bool hudButton(
  Renderer& r, UserCommand& userCmd, std::string text, float x, float y);
void hudLabel(
  Renderer& r, UserCommand& userCmd, std::string text, float x, float y);

#endif
