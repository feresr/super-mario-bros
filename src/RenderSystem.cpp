#include "RenderSystem.h"

void RenderSystem::tick(World* world) {
    //todo: placeholder test code
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    for (auto entity : world->find<TransformComponent>()) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect{entity->get<TransformComponent>()->x, entity->get<TransformComponent>()->y, 30, 30};
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

void RenderSystem::onAddedToWorld(World* world) {

}

void RenderSystem::onRemovedFromWorld(World* world) {

}

RenderSystem::RenderSystem(SDL_Window* window) {
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer) {
        SDL_Log("Could not create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Could not initialize RenderSystem");
    }
}

RenderSystem::~RenderSystem() {
    SDL_DestroyRenderer(renderer);
}

TransformComponent::TransformComponent(int x, int y) : x(x), y(y) {}
