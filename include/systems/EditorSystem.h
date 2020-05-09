#pragma once

#include "ecs/ecs.h"
#include "SDL.h"
#include "SDL_image.h"
#include "TileType.h"
#include "Components.h"
#include "Constants.h"

struct InputState {
    int x{SNES_RESOLUTION_WIDTH / 2}, y{SNES_RESOLUTION_HEIGHT / 2};
    bool clicked{};
    bool performSave{};
    int pan{};
    int item = 0;
};

class EditorSystem : public System {

public:
    explicit EditorSystem() = default;

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~EditorSystem() override = default;

private:
    Entity* cursor{};

    InputState inputState;

    void saveToDisk();

    TileSetComponent* tileSet{};

    std::vector<TileType> tileTypes = {FLOOR, BLOCK, BRICK, BUSH_LEFT, GOOMBA, BUSH_CENTER, BUSH_RIGHT, CLOUD};
};

