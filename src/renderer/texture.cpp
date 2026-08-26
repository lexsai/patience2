// #include "texture.hpp"

// #include <glad/glad.h>


// Texture::Texture()
// {
//   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//   glGenTextures(1, &m_texture);
  
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, m_texture);

//   glTexImage2D(
//     GL_TEXTURE_2D,
//     0,
//     GL_RED,
//     glyph->bitmap.width, glyph->bitmap.rows,
//     0,
//     GL_RED,
//     GL_UNSIGNED_BYTE,
//     glyph->bitmap.buffer
//   );

//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   // FT_Set_Pixel_Sizes(face, 0, 24);

//   // for (unsigned char c = 0; c < 128; c++)
//   // {
//   //   if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
//   //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not load glyph for '%c'", c);
//   //     continue;
//   //   }

//   //   Texture texture{ face->glyph };
//   // }
// }

// // Texture::Texture(FT_GlyphSlot glyph)
// // {
// //   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// //   glGenTextures(1, &m_texture);
  
// //   glActiveTexture(GL_TEXTURE0);
// //   glBindTexture(GL_TEXTURE_2D, m_texture);

// //   glTexImage2D(
// //     GL_TEXTURE_2D,
// //     0,
// //     GL_RED,
// //     glyph->bitmap.width, glyph->bitmap.rows,
// //     0,
// //     GL_RED,
// //     GL_UNSIGNED_BYTE,
// //     glyph->bitmap.buffer
// //   );

// //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// // }

// Texture::~Texture()
// {
//   glDeleteTextures(1, &m_texture);
// }

// Texture::Texture(Texture&& t) noexcept
//   : m_texture{ t.m_texture }
// {
//   t.m_texture = 0;
// }

// Texture& Texture::operator=(Texture&& t) noexcept
// {
//   if (&t == this)
//   {
//     return *this;
//   }

//   m_texture = t.m_texture;
//   t.m_texture = 0;

//   return *this;
// }

// void Texture::use()
// {
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, m_texture);
// }
