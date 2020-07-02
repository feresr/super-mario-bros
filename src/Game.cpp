
#include "Game.h"

void Game::init(const char* title, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    Uint32 flags = fullscreen ? SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

    if (!window) {
        SDL_Log("Unable to create window SDL: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    isRunning = true;
    currentScene = new IntroScene(window);
}


void Game::update() {
    if (currentScene->isFinished()) {
        if (dynamic_cast<IntroScene*>(currentScene)) {
            delete currentScene;
            currentScene = new GameScene(window);
        } else {
            delete currentScene;
            currentScene = new IntroScene(window);
        }
    }
    currentScene->update();
}

bool Game::running() const { return isRunning; }

void Game::handleEvents() {

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            return;
        }
        currentScene->handleEvents(event);
        switch (event.type) {
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_E:
                        if (dynamic_cast<GameScene*>(currentScene)) {
                            delete currentScene;
                            currentScene = new EditorScene(window);
                        } else {
                            delete currentScene;
                            currentScene = new GameScene(window);
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
    delete currentScene;
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}