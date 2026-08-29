#ifndef PATIENCE_WINDOW_HPP
#define PATIENCE_WINDOW_HPP

#include <SDL3/SDL.h>
#include <glad/glad.h>

class Window
{
  SDL_Window *m_window;
  SDL_GLContext m_glContext;

public:
  int m_width;
  int m_height;

  Window();
  Window(int width, int height);
  ~Window();

  Window(const Window& other) = delete;
  Window& operator=(const Window& other) = delete;
  Window(Window&& other) = delete;
  Window& operator=(Window&& other) = delete;

  void swapBuffer();
};

#endif
