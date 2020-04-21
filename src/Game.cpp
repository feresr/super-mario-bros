#include "Game.h"
#include "RenderSystem.h"

int Game::W_HEIGHT = 200;
int Game::W_WIDTH = 100;

void Game::init(const char* title, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    Uint32 flags = fullscreen ? SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

    if (!window) {
        SDL_Log("Unable to create window SDL: %s", SDL_GetError());
        SDL_Quit();
        return;
    }
    SDL_GL_GetDrawableSize(window, &W_WIDTH, &W_HEIGHT);

    isRunning = true;

    Entity* e = world.create();
    e->assign<TransformComponent>(0, 4);
    Entity* e2 = world.create();
    e2->assign<TransformComponent>(0, 36);
    world.registerSystem(new RenderSystem(window));
}

void gameOver() {
    SDL_Delay(800);
}

void Game::update() {
    world.tick();
}

bool Game::running() const { return isRunning; }

void Game::handleEvents() {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) isRunning = false;
}

void Game::clean() {
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}