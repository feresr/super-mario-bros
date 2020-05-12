#pragma once

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "ecs/ecs.h"
#include "systems/EditorSystem.h"
#include "systems/RenderSystem.h"
#include "systems/EditorSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/TileSystem.h"
#include "systems/MapSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/PlayerSystem.h"
#include "Constants.h"

class Game {

public:

    void init(const char* title, int width, int height, bool fullscreen);

    void handleEvents();

    void update();

    void clean();

    [[nodiscard]] bool running() const;

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Event event;
    World world;
    EditorSystem* editorSystem;
};
