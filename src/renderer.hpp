#ifndef PATIENCE_RENDERER_HPP
#define PATIENCE_RENDERER_HPP

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "vertex_buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

struct Character
{
  Texture texture;
  unsigned int width;
  unsigned int height;
  int bearingX;
  int bearingY;
  long advance;
};

class Shader;

class Renderer
{
  std::unique_ptr<Shader> m_textShader{};
  std::unique_ptr<VertexArray> m_textVertexArray{};
  std::unique_ptr<VertexBuffer> m_textVertexBuffer{};

  // std::vector<Vertex> m_textVertices{};
  std::map<char, Character> m_characters{};

  void initText();
  // void addTextVertex(float x, float y, float u, float v);
public:
  Renderer();
  ~Renderer();

  Renderer(const Renderer& other) = delete;
  Renderer& operator=(const Renderer& other) = delete;
  Renderer(Renderer&& other) = delete;
  Renderer& operator=(Renderer&& other) = delete;


  void drawText(std::string text, float x, float y);
  // void drawEnd();
};

#endif
