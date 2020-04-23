#pragma once

#include "ecs/ecs.h"
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <iostream>

struct TransformComponent : public Component {
    TransformComponent(int x, int y, int w, int h);

    int x, y, w, h;

    ~TransformComponent() override = default;
};

struct TextureComponent : public Component {
    explicit TextureComponent(const std::string&& path);

    const std::string path;
    SDL_Texture* texture = nullptr;

    ~TextureComponent() override {
        std::cout << "Texture component deleted" << std::endl;
        SDL_DestroyTexture(texture);
    }
};

class RenderSystem : public System {

public:
    explicit RenderSystem(SDL_Window* window);

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~RenderSystem() override;

private:
    SDL_Texture* getTexture(TextureComponent& path);

    SDL_Renderer* renderer;
};
