#include "systems/TileSystem.h"

void TileSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void TileSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void TileSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}

void TileSystem::tick(World* world) {
    for (auto entity : world->find<BreakableComponent, BottomCollisionComponent>()) {
        auto breakable = entity->get<BreakableComponent>();

        if (entity->has<QuestionBlockComponent>()) {
            entity->remove<AnimationComponent>();
            entity->get<TextureComponent>()->id = QUESTION_BLOCK_TEXTURE_HIT;
        }
        if (!breakable->finished()) {
            entity->get<TransformComponent>()->y += (float) breakable->getHeight();
        } else {
            entity->remove<BottomCollisionComponent>();
            breakable->reset();

            if (entity->has<QuestionBlockComponent>()) {
                entity->remove<QuestionBlockComponent>();
                entity->remove<BreakableComponent>();
            }
        }
    }
}
