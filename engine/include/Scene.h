#pragma once
#include "ecs.h"

class Scene {
protected:
    World* world = new World();

public:
    virtual void update();

    virtual bool isFinished();

    virtual void handleEvents(SDL_Event& event);

    virtual ~Scene();
};