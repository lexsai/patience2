#include "sprite_atlas.hpp"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdexcept>

SpriteAtlas::SpriteAtlas(const char *imgPath)
{
  unsigned int texture;
  glGenTextures(1, &texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(imgPath, &width, &height, &nrChannels, STBI_rgb_alpha);
  if (data)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    glDeleteTextures(1, &texture);
    throw std::runtime_error("failed to load texture '%s'\n" + std::string(imgPath));
  }
  stbi_image_free(data);

  m_texture = texture;
  m_width = width;
  m_height = height;
}

SpriteAtlas::~SpriteAtlas()
{
  glDeleteTextures(1, &m_texture);
}

std::vector<Vertex> SpriteAtlas::generateSpriteVertices(
  SpriteAtlasSpecifier s, float x, float y, float spriteWidth, float spriteHeight)
{
  // for pixel perfect rendering
  x = std::round(x);
  y = std::round(y);

  if (spriteHeight== 0)
  {
    spriteHeight = spriteWidth * (s.size.y / static_cast<float>(s.size.x));
  }

  std::vector<Vertex> spriteVertices{};

  float atlasWidth = static_cast<float>(m_width);
  float atlasHeight = static_cast<float>(m_height);

  float left = s.botLeft.x / atlasWidth;  
  float right = (s.botLeft.x + s.size.x) / atlasWidth;
  float bot = s.botLeft.y / atlasHeight;  
  float top = (s.botLeft.y + s.size.y) / atlasHeight;
  
  spriteVertices.push_back({ x,               y,                left,  bot });
  spriteVertices.push_back({ x + spriteWidth, y + spriteHeight, right, top });
  spriteVertices.push_back({ x,               y + spriteHeight, left,  top });

  spriteVertices.push_back({ x,               y               , left,  bot });
  spriteVertices.push_back({ x + spriteWidth, y               , right, bot });
  spriteVertices.push_back({ x + spriteWidth, y + spriteHeight, right, top });

  return spriteVertices;
}

void SpriteAtlas::use()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture);
}
