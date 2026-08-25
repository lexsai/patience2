#include "renderer.hpp"
#include "shader.hpp"
#include "vertex_array.hpp"

#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Renderer::Renderer()
{
  initText();
}

Renderer::~Renderer()
{
}

void Renderer::drawText(std::string text, float x, float y)
{
  std::vector<Vertex> vertices = m_fontAtlas->generateTextVertices(text, x, y);
  for (Vertex vertex: vertices)
  {
    addTextVertex(vertex);
  }
}

void Renderer::drawEnd()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

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
    "assets/shaders/vert.glsl", 
    "assets/shaders/frag.glsl");
  
  m_textVertexArray = std::make_unique<VertexArray>();
  m_textVertexBuffer = std::make_unique<VertexBuffer>(*m_textVertexArray, 999);
}

void Renderer::addTextVertex(Vertex vertex)
{
  if (m_textVertices.size() == m_textVertexBuffer->m_maxVertices)
  {
    drawEnd();
  }

  m_textVertices.push_back(vertex);
}
