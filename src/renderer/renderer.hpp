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
#include "font_atlas.hpp"
#include "sprite_atlas.hpp"

constexpr int TILE_WIDTH = 48;

struct Tile;
class Shader;

class Renderer
{
  std::unique_ptr<FontAtlas> m_fontAtlas{};
  std::unique_ptr<Shader> m_textShader{};
  std::unique_ptr<VertexArray> m_textVertexArray{};
  std::unique_ptr<VertexBuffer> m_textVertexBuffer{};
  std::vector<Vertex> m_textVertices{};

  std::unique_ptr<SpriteAtlas> m_spriteAtlas{};
  std::unique_ptr<Shader> m_spriteShader{};
  std::unique_ptr<VertexArray> m_spriteVertexArray{};
  std::unique_ptr<VertexBuffer> m_spriteVertexBuffer{};
  std::vector<Vertex> m_spriteVertices{};
  int m_staticSpriteVertexCount{};

  void initSprite();
  void initText();
  void addTextVertex(Vertex vertex);
  void addSpriteVertex(Vertex vertex);

public:

  Renderer();
  ~Renderer();

  Renderer(const Renderer& other) = delete;
  Renderer& operator=(const Renderer& other) = delete;
  Renderer(Renderer&& other) = delete;
  Renderer& operator=(Renderer&& other) = delete;

  void drawText(std::string text, float x, float y);
  // leave spriteHeight as 0 to scale automatically according to width 
  void drawSprite(
    SpriteAtlasSpecifier s, float x, float y, float spriteWidth, float spriteHeight);
  void drawBegin();
  void drawEnd();

  void drawStaticSprite(
    SpriteAtlasSpecifier s, float x, float y, float spriteWidth, float spriteHeight);

  void renderText();
  void renderSprites();
};

#endif
