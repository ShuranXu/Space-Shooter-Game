#include <iostream>
#include <memory>
#include "game.h"
#include "renderer.h"
#include "player.h"
#include <cstdlib>

//define global variables
constexpr std::size_t kScreenWidth{1280};
constexpr std::size_t kScreenHeight{720};

SDL_Window *sdl_window = nullptr;
SDL_Renderer *sdl_renderer = nullptr;
std::unique_ptr<Renderer> renderer;
std::unique_ptr<Game> game;


static void createSDLWindowAndRender(SDL_Window **sdl_window, SDL_Renderer **sdl_renderer) {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    // Create Window
    *sdl_window = SDL_CreateWindow("Battle Arena", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                                kScreenHeight, SDL_WINDOW_SHOWN);

    if (nullptr == *sdl_window) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    // Create renderer
    *sdl_renderer = SDL_CreateRenderer(*sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == *sdl_renderer) {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(*sdl_window);
        SDL_Quit();
        exit(1);
    }
}

static void SDLCleanup() {

    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

int main() {

    createSDLWindowAndRender(&sdl_window, &sdl_renderer);
    std::unique_ptr<Player> player = std::make_unique<Player>(sdl_renderer);
    renderer = std::make_unique<Renderer>(sdl_window, sdl_renderer);
    game = std::make_unique<Game>(sdl_renderer, std::move(renderer), std::move(player));
    bool new_record = true;

    //register the program exit function
    atexit(SDLCleanup);

    while(true) {

        if(new_record){
            new_record = false;
            game->begin();
        }
        game->run();
        new_record = game->endWithRecord();
  }
  return 0;
}