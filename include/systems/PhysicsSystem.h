#pragma once

#include "ecs/ecs.h"
#include "Components.h"
#include "Constants.h"
#include <unordered_set>

class PhysicsSystem : public System {

public:
    explicit PhysicsSystem() = default;

    void onAddedToWorld(World* world) override {};

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override {};

    void handleEvent(SDL_Event& event) override {};

private:
};
