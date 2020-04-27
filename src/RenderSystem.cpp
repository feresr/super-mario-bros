#include "RenderSystem.h"
#include <filesystem>
#include <Components.h>


void RenderSystem::tick(World* world) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    auto dstRect = SDL_Rect();

    //find a specific component...
    //can I compose Compoments  TileComponent = TransformComponent + TextureComponent (is this even a good idea)
    auto camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    SDL_GetMouseState(&(camera->x), nullptr);

    for (auto entity : world->find<TransformComponent, TextureComponent>()) {
        auto transform = entity->get<TransformComponent>();
        dstRect.x = transform->left() - camera->left();
        dstRect.y = transform->top() - camera->top();
        dstRect.w = transform->w;
        dstRect.h = transform->h;

        SDL_RenderCopy(renderer, getTexture(*(entity->get<TextureComponent>())), nullptr, &dstRect);
    }
    SDL_RenderPresent(renderer);
}

void RenderSystem::onAddedToWorld(World* world) {
    world->create()->assign<CameraComponent>(GAME_RESOLUTION_WIDTH / 2,
                                             GAME_RESOLUTION_HEIGHT / 2,
                                             GAME_RESOLUTION_WIDTH,
                                             GAME_RESOLUTION_HEIGHT);
}

void RenderSystem::onRemovedFromWorld(World* world) {

}

RenderSystem::RenderSystem(SDL_Window* window, int gameResolutionWidth, int gameResolutionHeight)
        : GAME_RESOLUTION_WIDTH{gameResolutionWidth}, GAME_RESOLUTION_HEIGHT{gameResolutionHeight} {
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, render_flags);
    if (!renderer) {
        SDL_Log("Could not create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Could not initialize RenderSystem");
    }

    SDL_RenderSetLogicalSize(renderer, GAME_RESOLUTION_WIDTH, GAME_RESOLUTION_HEIGHT);
}

RenderSystem::~RenderSystem() {
    SDL_DestroyRenderer(renderer);
}

SDL_Texture* RenderSystem::getTexture(TextureComponent& textureComponent) {
    if (textureComponent.texture) return textureComponent.texture;
    std::cout << "Creating texture: " << textureComponent.path << std::endl;
    SDL_Surface* tempSurface = IMG_Load(textureComponent.path.c_str());
    if (!tempSurface) {
        std::cout << "Unable to load texture: " << textureComponent.path << " - " << IMG_GetError() << std::endl;
        return nullptr;
    }
    textureComponent.texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!textureComponent.texture) {
        std::cout << "Unable to create texture: " << textureComponent.path << " - " << IMG_GetError() << std::endl;
        return nullptr;
    }
    return textureComponent.texture;
}