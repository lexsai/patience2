#ifndef PATIENCE_FONT_ATLAS_HPP
#define PATIENCE_FONT_ATLAS_HPP

#include "vertex_buffer.hpp"

#include <glm/glm.hpp>
#include <ft2build.h> 
#include FT_FREETYPE_H

#include <vector>
#include <string>
#include <map>

struct FreeTypeContext
{
  FT_Library ft;
  FT_Face face;
};

struct Glyph
{
  glm::vec2 topLeftUV;
  glm::vec2 botRightUV;
  glm::ivec2 size;
  glm::ivec2 bearing;
  long advance;
};

class FontAtlas
{
  unsigned int m_texture{};

  std::map<char, Glyph> m_glyphByChar{};

  FreeTypeContext loadFont(const char *fontPath);

  glm::ivec2 determineAtlasSize(FreeTypeContext context);
  unsigned int allocateFontTexture(FreeTypeContext context, int width, int height);
  void freeFontTexture(unsigned int texture);

  void loadFontTexture(
    unsigned int texture, FreeTypeContext context, int width, int height);
  void unloadFont(FreeTypeContext context);
public:
  explicit FontAtlas(const char *fontPath);
  ~FontAtlas();

  FontAtlas(const FontAtlas& other) = delete;
  FontAtlas& operator=(const FontAtlas& other) = delete;
  FontAtlas(FontAtlas&& other) = delete;
  FontAtlas& operator=(FontAtlas&& other) = delete;

  std::vector<Vertex> generateTextVertices(std::string text, float x, float y);
  void use();
};

#endif
