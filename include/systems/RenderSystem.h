#pragma once

#include "ecs/ecs.h"
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <iostream>
#include "Components.h"
#include "TextureManager.h"
#include <filesystem>
#include <SDL_ttf.h>

class RenderSystem : public System {

public:
    explicit RenderSystem(SDL_Window* window, int gameResolutionWidth, int gameResolutionHeight);

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~RenderSystem() override;

private:
    SDL_Renderer* renderer;

    const int GAME_RESOLUTION_WIDTH;
    const int GAME_RESOLUTION_HEIGHT;
    TextureManager* textureManager{};
    CameraComponent* camera{};

    void renderEntities(std::vector<Entity*> entities);

    void renderText(std::vector<Entity*> entities);

    TTF_Font * font;
};
