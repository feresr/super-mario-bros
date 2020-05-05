#include "Game.h"

int main() {
    uint32_t frameStart, frameTime;

    Game* game = new Game();
    game->init("Super Mario Bros",
               SNES_RESOLUTION_WIDTH * ZOOM,
               SNES_RESOLUTION_HEIGHT * ZOOM,
               false);

    while (game->running()) {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        game->update();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DURATION) SDL_Delay(FRAME_DURATION - frameTime);
    }

    game->clean();
    delete game;
    return EXIT_SUCCESS;
}
