#pragma once

#include "ecs.h"

class FlagSystem : public System {

public:
    explicit FlagSystem(std::function<void(void)> gameOverCallback);

    void onAddedToWorld(World* world) override;

    ~FlagSystem() override = default;
private:

    std::function<void(void)> gameOverCallback;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;
};