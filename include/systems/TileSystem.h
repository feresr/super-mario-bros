#pragma once

#include "ecs/ecs.h"
#include "Components.h"
#include "Constants.h"

class TileSystem : public System {

public:
    TileSystem() = default;

    virtual void onAddedToWorld(World* world);

    virtual void tick(World* world);

    virtual void handleEvent(SDL_Event& event);

    virtual void onRemovedFromWorld(World* world);

    virtual ~TileSystem() = default;

};