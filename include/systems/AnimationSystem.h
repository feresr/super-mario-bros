#pragma once

#include "ecs/ecs.h"
#include "Components.h"

class AnimationSystem : public System {

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    ~AnimationSystem() = default;
};