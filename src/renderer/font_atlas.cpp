#include "font_atlas.hpp"

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <stdexcept>
#include <algorithm>

FontAtlas::FontAtlas(const char *fontPath)
{
  FreeTypeContext font = loadFont(fontPath);

  glm::ivec2 atlasSize = determineAtlasSize(font);

  m_texture = allocateFontTexture(font, atlasSize.x, atlasSize.y);
  loadFontTexture(m_texture, font, atlasSize.x, atlasSize.y);

  unloadFont(font);
}

FontAtlas::~FontAtlas()
{
  freeFontTexture(m_texture);
}

glm::ivec2 FontAtlas::determineAtlasSize(FreeTypeContext context)
{
  int atlasWidth = 0;
  int atlasHeight = 0;

  for (unsigned char c = 0; c < 128; c++)
  {
    if (FT_Load_Char(context.face, c, FT_LOAD_RENDER))
    {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not load glyph for '%c'", c);
      continue;
    }
    
    atlasWidth += context.face->glyph->bitmap.width;
    atlasHeight = std::max(
      atlasHeight, static_cast<int>(context.face->glyph->bitmap.rows));
  }
  return glm::ivec2{ atlasWidth, atlasHeight };
}

FreeTypeContext FontAtlas::loadFont(const char *fontPath)
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    throw std::runtime_error("could not initialise freetype lib");
  }
  FT_Face face;
  if (FT_New_Face(ft, fontPath, 0, &face))
  {
    FT_Done_FreeType(ft);
    throw std::runtime_error("could not load font face");
  }
  FT_Set_Pixel_Sizes(face, 0, 24);

  return { ft, face };
}

void FontAtlas::unloadFont(FreeTypeContext context)
{
  FT_Done_Face(context.face);
  FT_Done_FreeType(context.ft);
}

unsigned int FontAtlas::allocateFontTexture(
  FreeTypeContext context, int width, int height)
{
  unsigned int texture = 0;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(1, &texture);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RED,
    width, height,
    0,
    GL_RED,
    GL_UNSIGNED_BYTE,
    nullptr
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return texture;
}

void FontAtlas::loadFontTexture(
  unsigned int texture, FreeTypeContext context, int width, int height)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  int xOffset = 0;
  for (unsigned char c = 0; c < 128; c++)
  {
    if (FT_Load_Char(context.face, c, FT_LOAD_RENDER))
    {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not load glyph for '%c'", c);
      continue;
    }
    
    FT_GlyphSlot glyph = context.face->glyph;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // (xoffset, 0) is bottom left of img added to texture 
    glTexSubImage2D(
      GL_TEXTURE_2D,
      0, 
      xOffset, 0,
      glyph->bitmap.width, glyph->bitmap.rows,
      GL_RED, 
      GL_UNSIGNED_BYTE,
      glyph->bitmap.buffer
    );

    Glyph storedGlyph{};

    storedGlyph.topLeftUV = glm::vec2(
      static_cast<float>(xOffset) / width, 
      static_cast<float>(glyph->bitmap.rows) / height);
    storedGlyph.botRightUV = glm::vec2(
      (static_cast<float>(xOffset) + glyph->bitmap.width) / width, 
      0.0f);

    storedGlyph.size = glm::ivec2(glyph->bitmap.width, glyph->bitmap.rows);
    storedGlyph.bearing = glm::ivec2(glyph->bitmap_left, glyph->bitmap_top);
    storedGlyph.advance = glyph->advance.x;

    m_glyph_by_char.insert({c, storedGlyph});

    xOffset += glyph->bitmap.width;
  }
}

std::vector<Vertex> FontAtlas::generateTextVertices(
  std::string text, float x, float y)
{
  std::vector<Vertex> textVertices{};

  for (char character : text)
  {
    Glyph g = m_glyph_by_char.at(character);

    float xpos = x + g.bearing.x;
    float ypos = y - (g.size.y - g.bearing.y);

    // top right but tex coord is lower left to flipi -- flip y sign  
    float left = g.topLeftUV.x;
    float right = g.botRightUV.x;
    float top = g.topLeftUV.y;
    float bot = g.botRightUV.y;

    textVertices.push_back({ xpos,            ypos + g.size.y, left, bot });
    textVertices.push_back({ xpos,            ypos,             left, top });
    textVertices.push_back({ xpos + g.size.x, ypos,            right, top });

    textVertices.push_back({ xpos,            ypos + g.size.y, left, bot });
    textVertices.push_back({ xpos + g.size.x,             ypos, right, top });
    textVertices.push_back({ xpos + g.size.x, ypos + g.size.y, right, bot });

    x += g.advance >> 6;
  }
  return textVertices;
}

void FontAtlas::use()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture);
}

void FontAtlas::freeFontTexture(unsigned int texture)
{
  glDeleteTextures(1, &texture);
}
