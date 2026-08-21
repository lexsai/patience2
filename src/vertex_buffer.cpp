#include "vertex_buffer.hpp"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(int maxVertices)
  : m_maxVertices{ maxVertices }
{
  glGenBuffers(1, &m_vbo);
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
