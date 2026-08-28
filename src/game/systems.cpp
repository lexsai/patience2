#include "systems.hpp"
#include "map.hpp"

bool isColliding(Entity& e, Tilemap& tilemap)
{
  int leftTile = static_cast<int>(e.left()) / TILE_WIDTH;
  int rightTile = static_cast<int>(e.right()) / TILE_WIDTH;
  int topTile = static_cast<int>(e.top()) / TILE_WIDTH;
  int bottomTile = static_cast<int>(e.bottom()) / TILE_WIDTH;

  SDL_Log("left %d right %d top %d bot %d", leftTile, rightTile, topTile, bottomTile);

  if (leftTile < 0) leftTile = 0;
  if (rightTile < 0) rightTile = 0;
  if (topTile < 0) topTile = 0;
  if (bottomTile < 0) bottomTile = 0;

  if (leftTile >= tilemap.width) leftTile = tilemap.width - 1;
  if (rightTile >= tilemap.width) rightTile = tilemap.width - 1;
  if (topTile >= tilemap.height) topTile = tilemap.height - 1;
  if (bottomTile >= tilemap.height) topTile = tilemap.height - 1;

  for (int x = leftTile; x <= rightTile; x++)
  {
    for (int y = bottomTile; y <= topTile; y++)
    {
      Tile t = tilemap.tiles[y][x];
      if (t.solid)
      {
        return true;
      }
    }
  }
  SDL_Log("hm?");
  return false;
}

void updatePhysics(Game& game)
{
  for (auto& e: game.m_entityPool)
  {
    if (e.vx == 0 && e.vy == 0) continue;

    float prevX = e.x; 
    e.x += e.vx;
    if (isColliding(e, game.m_loadedMap.ground))
    {
      e.x = prevX;
    }

    float prevY = e.y; 
    e.y += e.vy;
    if (isColliding(e, game.m_loadedMap.ground))
    {
      e.y = prevY;
    }

    e.vx = 0;
    e.vy = 0;
  }
}

void updateEntityInteraction(Entity& e, Game& game, UserCommand& userCmd)
{
  if (!e.interactable || !userCmd.activate) return;

  Entity* p = game.m_entityPool.getEntity(game.m_player);
  if (!p) return;

  float playerCenterX = p->x + p->width / 2;
  float playerCenterY = p->y + p->height / 2;

  constexpr int INTERACTION_RANGE = 10;

  if (
    ((playerCenterX >= e.left() && playerCenterX <= e.right())  &&
    ((p->direction == Direction::Up && e.bottom() - p->top() <= INTERACTION_RANGE && e.bottom() - p->top() >= -e.height) || 
      (p->direction == Direction::Down && p->bottom() - e.top() <= INTERACTION_RANGE && p->bottom() - e.top() >= -e.height))) ||
    ((playerCenterY >= e.bottom() && playerCenterY <= e.top())  &&
    ((p->direction == Direction::Right && e.left() - p->right() <= INTERACTION_RANGE && e.left() - p->right() >= -e.width) ||
      (p->direction == Direction::Left && p->left() - e.right() <= INTERACTION_RANGE && p->x - e.right() >= -e.width))))
  {
    // TODO: better way to prevent activate from insta closing the dialogue.
    userCmd.activate = false;
    if (e.onInteract)
    {
      e.onInteract(game);
    }
  }
}

void updateEntityAnimation(Entity& e, Game& game)
{
  if (!e.hasAnimation) return;

  std::vector<Frame> frames = game.animations[e.currentAnimation];
  Frame currentFrame = frames[e.currentFrameIndex];
  
  e.sprite = currentFrame.sprite;
  
  e.animationTime += 0.015f;
  if (e.animationTime > currentFrame.duration) {
    e.animationTime = 0.0f;
    if (e.currentFrameIndex + 1 == frames.size()) {
      e.currentFrameIndex = 0;
    } else {
      e.currentFrameIndex += 1;
    }
  }
}
