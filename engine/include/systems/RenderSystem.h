#pragma once

#include "ecs.h"
#include "TextureManager.h"
#include "Components.h"
#include "SDL.h"

namespace Engine {

    class RenderSystem : public System {

    public:
        explicit RenderSystem(SDL_Window* window, int gameResolutionWidth, int gameResolutionHeight,
                              glm::vec3 clearColor = {});

        void onAddedToWorld(World* world) override;

        void setBackgroundColor(int r, int g, int b);

        void tick(World* world) override;

        void onRemovedFromWorld(World* world) override;

        ~RenderSystem() override;

    private:
        const int GAME_RESOLUTION_WIDTH;
        const int GAME_RESOLUTION_HEIGHT;
        TextureManager* textureManager{};
        Engine::CameraComponent* camera{};

        void renderEntity(Entity* entity, bool followCamera = true);

        void renderText(Entity* entities);

        SDL_GLContext glContext;

        glm::vec3 clearColor;
    };
}