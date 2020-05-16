#include "systems/RenderSystem.h"
#include <filesystem>
#include <Components.h>

auto dstRect = SDL_Rect();

void RenderSystem::tick(World* world) {
    SDL_RenderClear(renderer);

    renderEntities(world->find<GrowComponent, TransformComponent, TextureComponent>());
    renderEntities(world->find<TileComponent, TransformComponent, TextureComponent>());
    renderEntities(world->find<EnemyComponent, TransformComponent, TextureComponent>());
    renderEntities(world->find<CollectibleComponent, TransformComponent, TextureComponent>());
    renderEntities(world->find<PlayerComponent, TransformComponent, TextureComponent>());

    //Editor
    auto tileSetEntity = world->findFirst<TileSetComponent>();
    if (tileSetEntity) {
        auto tileSetComponent = tileSetEntity->get<TileSetComponent>();
        for (int x = 0; x < tileSetComponent->mapWidth; x++) {
            for (int y = 0; y < tileSetComponent->mapHeight; y++) {
                if (!tileSetComponent->get(x, y).texture) continue;
                dstRect.x = x * TILE_SIZE - camera->left();
                dstRect.y = y * TILE_SIZE - camera->top();
                textureManager->renderTexture(tileSetComponent->get(x, y).editor_texture, dstRect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void RenderSystem::onAddedToWorld(World* world) {
    auto* entity = world->create();
    SDL_SetRenderDrawColor(renderer, SKY_RED, SKY_GREEN, SKY_BLUE, 255);
    entity->assign<CameraComponent>(GAME_RESOLUTION_WIDTH / 2,
                                    GAME_RESOLUTION_HEIGHT / 2,
                                    GAME_RESOLUTION_WIDTH,
                                    GAME_RESOLUTION_HEIGHT);
    camera = entity->get<CameraComponent>();
}

void RenderSystem::onRemovedFromWorld(World* world) {
    world->destroy(world->findFirst<CameraComponent>());
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
    textureManager = new TextureManager{renderer};
    SDL_SetTextureBlendMode(textureManager->texture, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(renderer, GAME_RESOLUTION_WIDTH, GAME_RESOLUTION_HEIGHT);
}

RenderSystem::~RenderSystem() {
    delete textureManager;
    SDL_DestroyRenderer(renderer);
}

void RenderSystem::renderEntities(std::vector<Entity*> entities) {
    for (auto entity : entities) {
        auto transform = entity->get<TransformComponent>();
        dstRect.x = transform->left() - camera->left();
        dstRect.y = transform->top() - camera->top();
        dstRect.w = transform->w;
        dstRect.h = transform->h;

        auto texture = entity->get<TextureComponent>();
        textureManager->renderTexture(texture->id, dstRect, texture->flipH, texture->flipV);
    }
}
