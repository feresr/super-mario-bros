#pragma once

#include "ecs/ecs.h"
#include "Components.h"
#include "Constants.h"
#include "TextureManager.h"
#include "AABB.h"

class PlayerSystem : public System {

    Entity* player;
    CameraComponent* camera;

    enum ANIMATION_STATE {
        RUNNING,
        STANDING,
        DRIFTING,
        DUCKING,
        JUMPING
    };

    ANIMATION_STATE currentState = STANDING;

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    ~PlayerSystem() override = default;

private:
    void setAnimation(ANIMATION_STATE animationState);
    void eatMushroom();
};