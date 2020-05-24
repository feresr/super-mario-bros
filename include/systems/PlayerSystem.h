#pragma once

#include "ecs/ecs.h"
#include "Components.h"
#include "Constants.h"
#include "TextureManager.h"
#include "AABB.h"

class PlayerSystem : public System {

    std::function<void(void)> gameOverCallback;

    Entity* player;
    CameraComponent* camera;

    enum ANIMATION_STATE {
        BLINKING,
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

    void onGameOver(World* world, Entity* player);

    ~PlayerSystem() override = default;

    void setAnimation(ANIMATION_STATE animationState);

    void eatMushroom(World* world,  bool oneup = false);

public:
    PlayerSystem(std::function<void(void)> gameOverCallback) : gameOverCallback{gameOverCallback} {}
};