#ifndef PATIENCE_VERTEX_BUFFER_HPP
#define PATIENCE_VERTEX_BUFFER_HPP

#include <vector>

#include "vertex_array.hpp"

struct Vertex
{
  float xy[2];
  float uv[2];
};

class VertexBuffer
{
  unsigned int m_vbo;

public:
  const int m_maxVertices{};

  VertexBuffer() = delete;
  VertexBuffer(VertexArray& vertexArray, int maxVertices);
  ~VertexBuffer();

  VertexBuffer(const VertexBuffer& other) = delete;
  VertexBuffer& operator=(const VertexBuffer& other) = delete;
  VertexBuffer(VertexBuffer&& other) = delete;
  VertexBuffer& operator=(VertexBuffer&& other) = delete;

  void bind();
};

#endif
