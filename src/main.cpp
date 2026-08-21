#include <Windows.h>

#include "game_window.hpp"
#include "renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>

#include <stdexcept>
#include <exception>

int main(int argc, char** argv)
{
  try
  {
    Window window{ 640, 480 };
    Renderer renderer{};

    bool running = true;
    while (running)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_EVENT_QUIT:
            running = false;
            break;
        }
      }

      renderer.drawText(
        std::string("the black stars which hang in the sky over Carcosa."), 
        0, 160
      );
      window.swapBuffer();
    };
  } 
  catch(const std::exception& exception) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "exception in main loop: %s", exception.what());
    return EXIT_FAILURE;
  }
  catch(...) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "abnormal exception");
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
