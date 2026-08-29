#include "game_hud.hpp"
#include "game.hpp"

struct HudElement
{
  SpriteAtlasSpecifier sprite;
  float x;
  float y;

  float w;
  float h;

  float left() { return x; }
  float right() { return x + w; }
  float bottom() { return y; }
  float top() { return y + h; }

  float isClicked(UserCommand& userCmd)
  {
    return userCmd.clickOnce && 
      userCmd.mouseX >= left() && userCmd.mouseX <= right() &&
      userCmd.mouseY >= bottom() && userCmd.mouseY <= top();
  }

  float isHovered(UserCommand& userCmd)
  {
    return userCmd.mouseX >= left() && userCmd.mouseX <= right() &&
      userCmd.mouseY >= bottom() && userCmd.mouseY <= top();
  }
};

struct HudContext
{
  bool startContainer{};
  float beginOffsetX{};
  float beginOffsetY{};
  float offsetX{};
  float offsetY{};
  float containerWidth{};
  std::vector<HudElement> containers; 
};

HudContext gHudContext{};

void hudBegin()
{
  gHudContext = {};
}

void hudBeginContainer(Renderer& r, UserCommand& userCmd, float x, float y)
{
  gHudContext.offsetX = x;
  gHudContext.offsetY = y;
  gHudContext.beginOffsetX = x;
  gHudContext.beginOffsetY = y;
  gHudContext.containerWidth = 0.0f;
  gHudContext.startContainer = false;
}

bool hudButton(
  Renderer& r, UserCommand& userCmd, std::string text, float x, float y)
{
  constexpr float width = TILE_WIDTH * 4.0f;
  constexpr float height = static_cast<float>(TILE_WIDTH);

  if (gHudContext.startContainer)
  {
    gHudContext.offsetY += TILE_WIDTH;
  }
  gHudContext.startContainer = true;

  HudElement c = {
    {{96, 0}, {64, 16}},
    gHudContext.offsetX + x,
    gHudContext.offsetY + y,
    width,
    height,
  };

  if (c.isHovered(userCmd))
  {
    c.sprite = {{96, 16}, {64, 16}};
  }

  gHudContext.containerWidth = std::max(gHudContext.containerWidth, width);
  r.drawText(text, c.left() + 10.0f, c.bottom() + 18.0f);

  bool isClicked = false;
  if (c.isClicked(userCmd))
  {
    c.sprite = {{160, 16}, {64, 16}};
    userCmd.clickOnce = false;
    userCmd.click = false;
    isClicked = true;
  }

  gHudContext.containers.push_back(c);

  return isClicked;
}

void hudLabel(
  Renderer& r, UserCommand& userCmd, std::string text, float x, float y)
{
  constexpr float width = TILE_WIDTH * 4.0f;
  constexpr float height = static_cast<float>(TILE_WIDTH);

  if (gHudContext.startContainer)
  {
    gHudContext.offsetY += TILE_WIDTH;
  }
  gHudContext.startContainer = true;

  HudElement c = {
    {{160, 16}, {64, 16}},
    gHudContext.offsetX + x,
    gHudContext.offsetY + y,
    width, height,
  };

  gHudContext.containerWidth = std::max(gHudContext.containerWidth, width);
  r.drawText(text, c.left() + 10.0f, c.bottom() + 18.0f);

  gHudContext.containers.push_back(c);
}

void hudEndContainer(Renderer& r, UserCommand& userCmd)
{
  r.drawHudSprite({{96, 32}, {64, 32}}, 
    gHudContext.beginOffsetX, gHudContext.beginOffsetY, 
    gHudContext.offsetX - gHudContext.beginOffsetX + gHudContext.containerWidth, 
    gHudContext.offsetY - gHudContext.beginOffsetY);
  
  for (HudElement c: gHudContext.containers)
  {
    r.drawHudSprite(
      c.sprite,
      c.x, c.y,
      c.w, c.h);
  }
}
