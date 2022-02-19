#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "Game.h"

void runLoop(void* arg) {
    Game *game = (Game*)arg;
    game->handleEvents();
    game->update();
}

int main() {
    uint32_t frameStart, frameTime;

    Game* game = new Game();
    game->init("Super Mario Bros",
            SNES_RESOLUTION_WIDTH * ZOOM,
            SNES_RESOLUTION_HEIGHT * ZOOM,
            false);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(runLoop, game, 0, 1);
#else
    while (game->running()) {
        frameStart = SDL_GetTicks();
        runLoop(game);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DURATION) SDL_Delay(FRAME_DURATION - frameTime);
    }

#endif
    game->clean();
    delete game;
    return EXIT_SUCCESS;
}
