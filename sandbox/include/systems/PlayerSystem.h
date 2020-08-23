#pragma once

#include "ecs.h"
#include "AABB.h"

class PlayerSystem : public System {

    std::function<void(void)> gameOverCallback;

    Entity* player{};
    Engine::CameraComponent* camera{};

    enum ANIMATION_STATE {
        BLINKING,
        RUNNING,
        STANDING,
        DRIFTING,
        DUCKING,
        JUMPING
    };

    ANIMATION_STATE currentState = STANDING;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    void onGameOver(World* world, Entity* player);

    void setAnimation(ANIMATION_STATE animationState);

    void eatMushroom(World* world,  bool oneup = false);

public:
    explicit PlayerSystem(std::function<void(void)> gameOverCallback) : gameOverCallback{std::move(gameOverCallback)} {}

    void onAddedToWorld(World* world) override;

    ~PlayerSystem() override = default;
};