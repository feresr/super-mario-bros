#include "systems/RenderSystem.h"
#include <SDL_ttf.h>

auto dstRect = SDL_Rect();

TTF_Font* font;

namespace Engine {
    void RenderSystem::tick(World* world) {
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        //Todo: Render in correct order (z index). Use Layers
        //Todo: ender items that don't follow the camera (Te TextComponent, TransformComponent, TextureComponent
        world->find<TransformComponent, TextureComponent>([&](Entity* entity) { renderEntity(entity); });
        world->find<TextComponent, TransformComponent>([&](Entity* entity) { renderText(entity); });

        //Editor
        /*auto tileSetEntity = world->findFirst<TileSetComponent>();
        if (tileSetEntity) {
            auto tileSetComponent = tileSetEntity->get<TileSetComponent>();
            for (int x = 0; x < tileSetComponent->mapWidth; x++) {
                for (int y = 0; y < tileSetComponent->mapHeight; y++) {
                    if (!tileSetComponent->get(x, y).texture) continue;
                    dstRect.x = x * TILE_SIZE - camera->left();
                    dstRect.y = y * TILE_SIZE - camera->top();
                    dstRect.w = TILE_SIZE;
                    dstRect.h = TILE_SIZE;
                    textureManager->renderTexture(tileSetComponent->get(x, y).editor_texture, dstRect);
                }
            }
        }*/
        SDL_RenderPresent(renderer);
    }

    void RenderSystem::onAddedToWorld(World* world) {
        auto* entity = world->create();
        entity->assign<CameraComponent>(GAME_RESOLUTION_WIDTH / 2,
                                        GAME_RESOLUTION_HEIGHT / 2,
                                        GAME_RESOLUTION_WIDTH,
                                        GAME_RESOLUTION_HEIGHT);
        camera = entity->get<CameraComponent>();

        TTF_Init();
        font = TTF_OpenFont("assets/font.ttf", 25);
        TTF_SetFontKerning(font, 1);
    }

    void RenderSystem::onRemovedFromWorld(World* world) {
        world->destroy(world->findFirst<CameraComponent>());
        TTF_CloseFont(font);
        TTF_Quit();
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
        SDL_SetTextureBlendMode(textureManager->textureAtlas, SDL_BLENDMODE_BLEND);
        SDL_RenderSetLogicalSize(renderer, GAME_RESOLUTION_WIDTH, GAME_RESOLUTION_HEIGHT);
    }

    RenderSystem::~RenderSystem() {
        delete textureManager;
        SDL_DestroyRenderer(renderer);
    }


    void RenderSystem::renderEntity(Entity* entity, bool followCamera) {
        auto transform = entity->get<TransformComponent>();
        auto texture = entity->get<TextureComponent>();
        if (followCamera) {
            dstRect.x = transform->left() - camera->left() + texture->offSetX;
            dstRect.y = transform->top() - camera->top() + texture->offSetY;
        } else {
            dstRect.x = transform->left() + texture->offSetX;
            dstRect.y = transform->top() + texture->offSetY;
        }

        dstRect.w = texture->w > 0 ? texture->w : transform->w;
        dstRect.h = texture->h > 0 ? texture->h : transform->h;

        textureManager->renderTexture(texture->id, dstRect, texture->flipH, texture->flipV);
    }

    void RenderSystem::renderText(Entity* text) {
        auto textComponent = text->get<TextComponent>();
        auto transformComponent = text->get<TransformComponent>();
        if (!textComponent->texture) {
            SDL_Color color = {255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Solid(font, textComponent->text.c_str(), color);
            textComponent->texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }

        dstRect.x = transformComponent->left();
        dstRect.y = transformComponent->top();

        dstRect.w = transformComponent->w;
        dstRect.h = transformComponent->h;

        SDL_RenderCopy(renderer, textComponent->texture, nullptr, &dstRect);
    }

    void RenderSystem::setBackgroundColor(int r, int g, int b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
}