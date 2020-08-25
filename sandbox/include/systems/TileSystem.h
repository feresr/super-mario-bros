#pragma once

#include "GameComponents.h"

class TileSystem : public System {

public:
    TileSystem() = default;

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    ~TileSystem() override = default;

private:
    TileMapComponent* tileMap{};

};