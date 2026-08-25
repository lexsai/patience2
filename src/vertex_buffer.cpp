#include "vertex_buffer.hpp"

#include <glad/glad.h>
#include <stdexcept>

VertexBuffer::VertexBuffer(VertexArray& vertexArray, int maxVertices)
  : m_maxVertices{ maxVertices }
{
  if (maxVertices % 3 != 0)
  {
    throw std::runtime_error("maxVertices should be divisible by 3");
  }

  vertexArray.bind();

  glGenBuffers(1, &m_vbo);

  // functions can not depend upon bind mounts being bound previous to call
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, xy));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &m_vbo);
}

void VertexBuffer::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}
