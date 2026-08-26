#include "shader.hpp"

#include <glad/glad.h>
#include <glm/ext.hpp>
#include <SDL3/SDL.h>

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace
{
  std::string readFile(std::string_view path)
  {
    std::ifstream input{ std::string(path) };
    if (!input.is_open())
    {
      throw std::runtime_error("could not read file");
    }
    
    std::stringstream stream;
    stream << input.rdbuf();
    input.close();
    return stream.str();
  }
}

Shader::Shader(std::string_view vertPath, std::string_view fragPath)
{
  GLint success;
  char infoLog[512];

  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertString = readFile(vertPath);
  const char *vertSource = vertString.c_str();
  glShaderSource(vertShader, 1, &vertSource, nullptr);
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "vert shader error: %s", infoLog);
  }

  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragString = readFile(fragPath);
  const char *fragSource = fragString.c_str();
  glShaderSource(fragShader, 1, &fragSource, nullptr);
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "frag shader error: %s", infoLog);
  }

  m_shaderProgram = glCreateProgram();
  glAttachShader(m_shaderProgram, fragShader);
  glAttachShader(m_shaderProgram, vertShader);
  glLinkProgram(m_shaderProgram);
  glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "m_shaderProgram link error: %s", infoLog);
  }

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

Shader::~Shader()
{
  glDeleteProgram(m_shaderProgram);
}

void Shader::use()
{
  glUseProgram(m_shaderProgram);
}

void Shader::uniformMatrix4f(const char *uniform, glm::mat4 matrix) {
  glUniformMatrix4fv(
    glGetUniformLocation(m_shaderProgram, uniform), 
    1,
    GL_FALSE,
    glm::value_ptr(matrix)
  );
}

 void Shader::uniformInt(const char *uniform, int value) {
  glUniform1i(
    glGetUniformLocation(m_shaderProgram, uniform), 
    value
  );
}
