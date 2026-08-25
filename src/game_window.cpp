#include "game_window.hpp"

#include <stdexcept>

Window::Window()
  : Window{ 640, 480 } {}

Window::Window(int width, int height) 
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  m_window = SDL_CreateWindow("test", 640, 480, SDL_WINDOW_OPENGL);
  if (!m_window)
  {
    throw std::runtime_error("could not init sdl window");
  }

  m_glContext = SDL_GL_CreateContext(m_window);
  if (!m_glContext)
  {
    SDL_DestroyWindow(m_window);
    throw std::runtime_error("could not create GL context");
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    throw std::runtime_error("could not load gl loader");
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
  SDL_GL_DestroyContext(m_glContext);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void Window::swapBuffer() {
  SDL_GL_SwapWindow(m_window);
}
