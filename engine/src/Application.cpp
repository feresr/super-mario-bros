#include "Application.h"
#include <SDL.h>

Engine::Application* Engine::Application::instance = nullptr;

Engine::Application::Application(const std::string& name,
                                 int width, int height,
                                 bool fullScreen) : appName{name} {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }
    Uint32 flags = fullScreen ? SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow(name.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height,
                              flags);
    if (!window) {
        SDL_Log("Unable to create window SDL: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    isRunning = true;
    instance = this;
};

Engine::Application::~Application() {
    delete scene;
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}


constexpr int FPS = 60;
constexpr int FRAME_DURATION = 1000 / FPS;

void Engine::Application::run() {
    uint32_t frameStart, frameTime;
    while (isRunning) {
        frameStart = SDL_GetTicks();
        if (scene->isFinished()) {
            delete scene;
            scene = OnNextScene();
        }
        while (SDL_PollEvent(&event)) handleEvent(event);
        scene->update();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DURATION) SDL_Delay(FRAME_DURATION - frameTime);
    }
}

void Engine::Application::handleEvent(SDL_Event& e) {}

