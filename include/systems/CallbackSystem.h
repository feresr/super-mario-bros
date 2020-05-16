#pragma once

#include "ecs/ecs.h"
#include "Components.h"

class CallbackSystem : public System {

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    ~CallbackSystem() override = default;
};