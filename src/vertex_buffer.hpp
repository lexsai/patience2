#ifndef PATIENCE_VERTEX_BUFFER_HPP
#define PATIENCE_VERTEX_BUFFER_HPP

#include <vector>

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
  explicit VertexBuffer(int maxVertices);
  ~VertexBuffer();

  VertexBuffer(const VertexBuffer& other) = delete;
  VertexBuffer& operator=(const VertexBuffer& other) = delete;
  VertexBuffer(VertexBuffer&& other) = delete;
  VertexBuffer& operator=(VertexBuffer&& other) = delete;

  void bind();
  void addVertices(std::vector<Vertex> vertices);
};

#endif
