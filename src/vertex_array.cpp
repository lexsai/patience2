#include "vertex_array.hpp"

#include <glad/glad.h>

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
}

VertexArray::~VertexArray()
{
  glDeleteVertexArrays(1, &m_vao);
}

void VertexArray::bind()
{
  glBindVertexArray(m_vao);
}
