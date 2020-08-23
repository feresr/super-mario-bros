#pragma once

#include "ecs.h"


class ScoreSystem : public System {

public:
    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    ~ScoreSystem() override = default;

private:
    Entity* scoreEntity;
    Entity* coinsEntity;
    Entity* timeLeftEntity;

    int score = 0;
    int coins = 0;
    int time = 255 * 60;
};