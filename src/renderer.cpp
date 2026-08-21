#include "renderer.hpp"
#include "shader.hpp"
#include "vertex_array.hpp"

#include <stdexcept>

#include <ft2build.h> 
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Renderer::Renderer()
{
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    throw std::runtime_error("could not load gl loader");
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  initText();
  m_textShader = std::make_unique<Shader>(
    "assets/shaders/vert.glsl", 
    "assets/shaders/frag.glsl");
  
  m_textVertexArray = std::make_unique<VertexArray>();
  m_textVertexArray->bind();
  
  m_textVertexBuffer = std::make_unique<VertexBuffer>(1000);
  m_textVertexBuffer->bind();
}

Renderer::~Renderer()
{
}

void Renderer::drawText(std::string text, float x, float y)
{
  m_textShader->use();
  m_textVertexArray->bind();

  for (char character : text)
  {
    Character *c = &m_characters.at(character);

    float xpos = x + c->bearingX;
    float ypos = y - (c->height - c->bearingY);

    // top right but tex coord is lower left to flipi -- flip y sign  
    // addTextVertex(xpos,            ypos + c->height, 0.0f, 0.0f);
    // addTextVertex(xpos,            ypos,             0.0f, 1.0f);
    // addTextVertex(xpos + c->width, ypos,             1.0f, 1.0f);

    // addTextVertex(xpos,            ypos + c->height, 0.0f, 0.0f);
    // addTextVertex(xpos + c->width,             ypos, 1.0f, 1.0f);
    // addTextVertex(xpos + c->width, ypos + c->height, 1.0f, 0.0f);

    float vertices[6][4] = {
      { xpos,            ypos + c->height, 0.0f, 0.0f },
      { xpos,            ypos,             0.0f, 1.0f },
      { xpos + c->width, ypos,             1.0f, 1.0f },
      { xpos,            ypos + c->height, 0.0f, 0.0f },
      { xpos + c->width,             ypos, 1.0f, 1.0f },
      { xpos + c->width, ypos + c->height, 1.0f, 0.0f }
    };

    c->texture.use();

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 mvp = projection * view;
    m_textShader->uniformMatrix4f("mvp", mvp);
    m_textShader->uniformInt("uTexture", 0);

    m_textVertexBuffer->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += c->advance >> 6;
  }
}

// void Renderer::drawEnd()
// {
//   m_textShader->use();
//   m_textVertexArray->bind();


//   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//   glClear(GL_COLOR_BUFFER_BIT);

// }

void Renderer::initText()
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    throw std::runtime_error("could not initialise freetype lib");
  }

  FT_Face face;
  if (FT_New_Face(ft, "assets/LinLibertine_R.ttf", 0, &face))
  {
    FT_Done_FreeType(ft);
    throw std::runtime_error("could not load font face");
  }

  FT_Set_Pixel_Sizes(face, 0, 24);

  for (unsigned char c = 0; c < 128; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not load glyph for '%c'", c);
      continue;
    }

    Texture texture{ face->glyph };
    Character character{
      std::move(texture),
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      face->glyph->bitmap_left,
      face->glyph->bitmap_top,
      face->glyph->advance.x
    };
    m_characters.insert({c, std::move(character)});
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

// void Renderer::addTextVertex(float x, float y, float u, float v)
// {
//   if (m_textVertices.size() == m_textVertexBuffer->m_maxVertices)
//   {
//     drawEnd();
//   }

//   m_textVertices.push_back(Vertex {
//     x, y, u, v  
//   });
// }
