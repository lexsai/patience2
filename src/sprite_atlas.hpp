
#ifndef PATIENCE_SPRITE_ATLAS_HPP
#define PATIENCE_SPRITE_ATLAS_HPP

#include "vertex_buffer.hpp"

#include <glm/glm.hpp>
#include <ft2build.h> 
#include FT_FREETYPE_H

#include <vector>
#include <string>
#include <map>

struct SpriteAtlasSpecifier {
  glm::ivec2 botLeft;
  glm::ivec2 size;
};

class SpriteAtlas
{
  unsigned int m_texture{};
  int m_width{};
  int m_height{};

public:
  explicit SpriteAtlas(const char *imgPath);
  ~SpriteAtlas();

  SpriteAtlas(const SpriteAtlas& other) = delete;
  SpriteAtlas& operator=(const SpriteAtlas& other) = delete;
  SpriteAtlas(SpriteAtlas&& other) = delete;
  SpriteAtlas& operator=(SpriteAtlas&& other) = delete;

  std::vector<Vertex> generateSpriteVertices(
    SpriteAtlasSpecifier s, float x, float y, float width, float height);
  void use();
};

#endif
