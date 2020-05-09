#include "systems/AnimationSystem.h"

void AnimationSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void AnimationSystem::tick(World* world) {
    auto entities = world->find<AnimationComponent, TextureComponent>();
    for (auto entity : entities) {
        auto animation = entity->get<AnimationComponent>();
        auto texture = entity->get<TextureComponent>();
        animation->counter++;
        if (animation->counter >= animation->duration) {
            if (animation->flipH) texture->flipH = !texture->flipH;
            if (animation->flipV) texture->flipV = !texture->flipV;
            animation->counter = 0;
            animation->currentTexture = (animation->currentTexture + 1) % animation->textures.size();
            texture->id = animation->textures[animation->currentTexture];
        }
    }
}

void AnimationSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void AnimationSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}
