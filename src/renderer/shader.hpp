#ifndef PATIENCE_SHADER_HPP
#define PATIENCE_SHADER_HPP

#include <ft2build.h> 
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include <string_view>

class Shader
{
  unsigned int m_shaderProgram;
public:
  Shader() = delete;
  Shader(std::string_view vertPath, std::string_view fragPath);
  ~Shader();

  Shader(const Shader& other) = delete;
  Shader& operator=(const Shader& other) = delete;
  Shader(Shader&& other) noexcept = delete;
  Shader& operator=(Shader&& other) noexcept = delete;

  void use();
  void uniformMatrix4f(const char* uniform, glm::mat4 matrix);
  void uniformInt(const char* uniform, int value);
};

#endif
