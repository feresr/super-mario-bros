#pragma once

#include "ecs/ecs.h"
#include <Components.h>

class EnemySystem : public System {

public:
    explicit EnemySystem() = default;

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~EnemySystem() override = default;
};

