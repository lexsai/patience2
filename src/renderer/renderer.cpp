#include "renderer.hpp"
#include "shader.hpp"
#include "vertex_array.hpp"
#include "../game/map.hpp"

#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Renderer::Renderer()
{
  initText();
  initSprite();
}

Renderer::~Renderer()
{
}

void Renderer::drawSprite(
  SpriteAtlasSpecifier s, float x, float y, float spriteWidth, float spriteHeight)
{
  if (s.size.x == 0 || s.size.y == 0) return;

  std::vector<Vertex> vertices = m_spriteAtlas->generateSpriteVertices(
    s, x, y, spriteWidth, spriteHeight
  );
  for (Vertex vertex: vertices)
  {
    addSpriteVertex(vertex);
  }
}

void Renderer::drawStaticSprite(
  SpriteAtlasSpecifier s, float x, float y, float spriteWidth, float spriteHeight)
{
  std::vector<Vertex> vertices = m_spriteAtlas->generateSpriteVertices(
    s, x, y, spriteWidth, spriteHeight);

  glBufferSubData(
    GL_ARRAY_BUFFER, 
    m_staticSpriteVertexCount * sizeof(Vertex), 
    vertices.size() * sizeof(Vertex), 
    vertices.data()
  );
  m_staticSpriteVertexCount += static_cast<int>(vertices.size());
}

void Renderer::drawText(std::string text, float x, float y)
{
  std::vector<Vertex> vertices = m_fontAtlas->generateTextVertices(text, x, y);
  for (Vertex vertex: vertices)
  {
    addTextVertex(vertex);
  }
}

void Renderer::drawBegin()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawEnd()
{
  renderSprites();
  renderText();
}

void Renderer::renderSprites()
{
  m_spriteShader->use();
  m_spriteAtlas->use();

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
  glm::mat4 mvp = projection * view;
  m_spriteShader->uniformMatrix4f("mvp", mvp);
  m_spriteShader->uniformInt("uTexture", 0);

  m_spriteVertexArray->bind();
  m_spriteVertexBuffer->bind();
  glBufferSubData(
    GL_ARRAY_BUFFER, 
    m_staticSpriteVertexCount * sizeof(Vertex), 
    m_spriteVertices.size() * sizeof(Vertex), 
    m_spriteVertices.data()
  );
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_spriteVertices.size() + m_staticSpriteVertexCount));

  m_spriteVertices.clear();
}

void Renderer::initSprite()
{
  m_spriteAtlas = std::make_unique<SpriteAtlas>("assets/teto.png");
  m_spriteShader = std::make_unique<Shader>(
    "assets/shaders/sprite/vert.glsl", 
    "assets/shaders/sprite/frag.glsl");
  
  m_spriteVertexArray = std::make_unique<VertexArray>();
  m_spriteVertexBuffer = std::make_unique<VertexBuffer>(*m_spriteVertexArray, 30000);
}

void Renderer::renderText()
{
  m_textShader->use();
  m_textVertexArray->bind();
  m_textVertexBuffer->bind();
  m_fontAtlas->use();

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
  glm::mat4 mvp = projection * view;
  m_textShader->uniformMatrix4f("mvp", mvp);
  m_textShader->uniformInt("uTexture", 0);

  glBufferSubData(GL_ARRAY_BUFFER, 0, m_textVertices.size() * sizeof(Vertex), m_textVertices.data());
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_textVertices.size()));

  m_textVertices.clear();
}

void Renderer::initText()
{
  m_fontAtlas = std::make_unique<FontAtlas>("assets/LinLibertine_R.ttf");
  m_textShader = std::make_unique<Shader>(
    "assets/shaders/text/vert.glsl", 
    "assets/shaders/text/frag.glsl");
  
  m_textVertexArray = std::make_unique<VertexArray>();
  m_textVertexBuffer = std::make_unique<VertexBuffer>(*m_textVertexArray, 30000);
}

void Renderer::addTextVertex(Vertex vertex)
{
  if (m_textVertices.size() == m_textVertexBuffer->m_maxVertices)
  {
    SDL_Log("notice: reached max text vertices, flushing buffers...");
    // render both to maintain sprite order
    renderSprites();
    renderText();
  }

  m_textVertices.push_back(vertex);
}

void Renderer::addSpriteVertex(Vertex vertex)
{
  if (m_spriteVertices.size() == m_spriteVertexBuffer->m_maxVertices)
  {
    SDL_Log("reached max sprites vertices, flushing buffers...");
    // render both to maintain sprite order
    renderSprites();
    renderText();
  }
  m_spriteVertices.push_back(vertex);
}
