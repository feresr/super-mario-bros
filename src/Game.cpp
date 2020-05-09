#include <systems/AnimationSystem.h>
#include <systems/PlayerSystem.h>
#include "Game.h"

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

    isRunning = true;
    world.registerSystem(new RenderSystem(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT));
    world.registerSystem(new PlayerSystem());
    world.registerSystem(new MapSystem());
    world.registerSystem(new AnimationSystem());
    world.registerSystem(new PhysicsSystem());
}

void gameOver() {
    SDL_Delay(800);
}

void Game::update() {
    world.tick();
}

bool Game::running() const { return isRunning; }

void Game::handleEvents() {

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            return;
        }
        world.handleEvent(event);
        switch (event.type) {
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_E:
                        if (editorSystem) {
                            world.unregisterSystem(editorSystem);
                            editorSystem = nullptr;
                        } else {
                            editorSystem = new EditorSystem();
                            world.registerSystem(editorSystem);
                        }
                        break;
                    default:
                        break;
                }
                break;
        }
    }

}

void Game::clean() {
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}