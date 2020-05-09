#pragma once

#include "ecs/ecs.h"
#include "Components.h"
#include "Constants.h"

class PlayerSystem : public System {

    Entity* player;

    enum ANIMATION_STATE {
        RUNNING,
        STANDING,
        DRIFTING,
        JUMPING
    };

    ANIMATION_STATE currentState = STANDING;

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    void setAnimation(ANIMATION_STATE animationState);

    ~PlayerSystem() override = default;

};