#ifndef PATIENCE_TEXTURE_HPP
#define PATIENCE_TEXTURE_HPP

#include <ft2build.h> 
#include FT_FREETYPE_H

class Texture
{
  unsigned int m_texture;
public:
  Texture() = delete;
  Texture(FT_GlyphSlot glyph);
  ~Texture();

  Texture(const Texture& other) = delete;
  Texture& operator=(const Texture& other) = delete;
  Texture(Texture&& other) noexcept;
  Texture& operator=(Texture&& other) noexcept;

  void use();
};

#endif
