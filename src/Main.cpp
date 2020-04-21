#include "Game.h"

constexpr int FPS = 60;
constexpr int frameDuration = 1000 / FPS;

int main() {
    Uint32 frameStart, frameTime;

    Game* game = new Game();
    game->init("Super Mario Bros", 640, 480, false);

    while (game->running()) {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        game->update();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDuration) SDL_Delay(frameDuration - frameTime);
    }

    game->clean();
    delete game;
    return EXIT_SUCCESS;
}
