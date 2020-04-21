#pragma once

#include "ecs/ecs.h"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class TransformComponent;

class RenderSystem : public System {

public:
    explicit RenderSystem(SDL_Window* window);

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~RenderSystem() override;

private:
    SDL_Renderer* renderer;
};

class TransformComponent : public Component {
public:
    TransformComponent(int x, int y);

    int x;
    int y;
};