
#ifndef PATIENCE_VERTEX_ARRAY_HPP
#define PATIENCE_VERTEX_ARRAY_HPP

class VertexArray
{
  unsigned int m_vao;

public:
  VertexArray();
  ~VertexArray();

  VertexArray(const VertexArray& other) = delete;
  VertexArray& operator=(const VertexArray& other) = delete;
  VertexArray(VertexArray&& other) = delete;
  VertexArray& operator=(VertexArray&& other) = delete;

  void bind();
};

#endif
