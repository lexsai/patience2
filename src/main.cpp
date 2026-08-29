#include <Windows.h>

#include "game_window.hpp"
#include "renderer/renderer.hpp"
#include "game/game.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>

#include <stdexcept>
#include <exception>

int main(int argc, char** argv)
{
  try
  {
    // Window window{ 640, 480 };
    Window window{};
    Renderer renderer{ window };
    std::unique_ptr<Game> gamePtr{ new Game{ renderer } };

    UserCommand userCmd{};

    float fps = 0;
    bool running = true;
    while (running)
    {
      uint64_t start = SDL_GetPerformanceCounter();

      float mouseX, mouseY;
      SDL_GetMouseState(&mouseX, &mouseY);
      gamePtr->updateUserCmdMousePos(userCmd, mouseX, mouseY);

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_EVENT_QUIT:
            running = false;
            break;
          case SDL_EVENT_KEY_DOWN:
            gamePtr->updateUserCmd(userCmd, event.key.scancode, true);
            break;
          case SDL_EVENT_KEY_UP:
            gamePtr->updateUserCmd(userCmd, event.key.scancode, false);
            break;
          case SDL_EVENT_MOUSE_BUTTON_DOWN:
            gamePtr->updateUserCmdClick(userCmd, 
              event.button.button == SDL_BUTTON_LEFT, true);
            break;
          case SDL_EVENT_MOUSE_BUTTON_UP:
            gamePtr->updateUserCmdClick(userCmd, 
              event.button.button == SDL_BUTTON_LEFT, false);
            break;
        }
      }

      renderer.drawBegin();

      gamePtr->update(userCmd, renderer);

      renderer.drawText(
        std::string("FPS: ") + std::to_string(fps), 
        0, 0
      );

      renderer.drawEnd();
      window.swapBuffer(); 

      uint64_t end = SDL_GetPerformanceCounter();
      fps = static_cast<float>(SDL_GetPerformanceFrequency()) / (end - start);
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
