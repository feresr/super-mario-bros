#include "systems/TileSystem.h"

void TileSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
    tileMap = world->findFirst<TileMapComponent>()->get<TileMapComponent>();
}

void TileSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void TileSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
    tileMap = nullptr;
}

void createMushroom(World* world, Entity* block) {
    auto mushroom = world->create();
    mushroom->assign<TextureComponent>(TextureId::MUSHROOM);
    mushroom->assign<CollectibleComponent>();
    mushroom->assign<TransformComponent>(
            block->get<TransformComponent>()->left(),
            block->get<TransformComponent>()->top(),
            TILE_SIZE,
            TILE_SIZE
    );
    mushroom->assign<GrowComponent>();
}

void createCoin(World* world, Entity* block) {
    auto coin = world->create();
    coin->assign<TextureComponent>(TextureId::COIN_1);

    coin->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::COIN_1, TextureId::COIN_2, TextureId::COIN_3, TextureId::COIN_4
            }, 5);

    coin->assign<TransformComponent>(
            block->get<TransformComponent>()->left(),
            block->get<TransformComponent>()->top(),
            TILE_SIZE,
            TILE_SIZE
    );
    coin->assign<GravityComponent>();
    coin->assign<TileComponent>();
    coin->assign<KineticComponent>(0.0f, -20.0f);
    coin->assign<CallbackComponent>([=]() { coin->clearComponents(); }, 20);
}

void TileSystem::tick(World* world) {

    for (auto entity : world->find<BottomCollisionComponent, TransformComponent, KineticComponent>()) {
        auto transform = entity->get<TransformComponent>();
        auto bottomTile = tileMap->get(
                (int) (transform->getCenterX() / TILE_SIZE),
                (int) (transform->getCenterY() / TILE_SIZE) + 1
        );
        if (bottomTile && bottomTile->has<BreakableComponent>()) {
            if (bottomTile->get<BreakableComponent>()->hit) {
                auto kinetic = entity->get<KineticComponent>();
                kinetic->speedY = -2.5f;
                if (entity->has<WalkComponent>()) entity->get<WalkComponent>()->speed *= -1.0f;
                kinetic->speedX *= -1;
            }
        }
    }

    //WALKABLE
    auto entities = world->find<WalkComponent, LeftCollisionComponent>();
    for (auto entity : entities) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = std::abs(walkComponent->speed);
    }

    for (auto entity : world->find<WalkComponent, RightCollisionComponent>()) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = -std::abs(walkComponent->speed);
    }

    for (auto entity : world->find<WalkComponent, KineticComponent>()) {
        entity->get<KineticComponent>()->speedX = entity->get<WalkComponent>()->speed;
    }

    //Growing mushrooms
    for (auto entity : world->find<GrowComponent, TransformComponent>()) {
        auto grow = entity->get<GrowComponent>();
        if (!grow->finished()) {
            entity->get<TransformComponent>()->y -= MUSHROOM_GROW_SPEED;
        } else {
            entity->remove<GrowComponent>();
            entity->assign<WalkComponent>(MUSHROOM_MOVE_SPEED);
            entity->assign<SolidComponent>();
            entity->assign<TileComponent>();
            entity->assign<KineticComponent>();
            entity->assign<GravityComponent>();
        }
    }

    for (auto entity : world->find<BreakableComponent, BottomCollisionComponent>()) {
        entity->get<BreakableComponent>()->hit = true;
    }

    for (auto entity : world->find<BreakableComponent>()) {
        auto breakable = entity->get<BreakableComponent>();
        if (!breakable->hit) continue;

        if (entity->has<QuestionBlockComponent>()) {
            entity->remove<AnimationComponent>();
            entity->get<TextureComponent>()->id = TextureId::QUESTION_BLOCK_OFF;
        }

        if (!breakable->finished()) {
            entity->get<TransformComponent>()->y += (float) breakable->getHeight();
        } else {
            breakable->reset();
            entity->get<BreakableComponent>()->hit = false;
            if (entity->has<QuestionBlockComponent>()) {
                if (entity->get<QuestionBlockComponent>()->spawn) createMushroom(world, entity);
                if (entity->get<QuestionBlockComponent>()->coin) createCoin(world, entity);
                entity->remove<QuestionBlockComponent>();
                entity->remove<BreakableComponent>();
            }
        }
    }

    for (auto destroy : world->find<DestroyDelayedComponent>()) {
        if (destroy->get<DestroyDelayedComponent>()->shouldDestroy()) {
            world->destroy(destroy);
        }
    }

    for (auto entity : world->findAny<
            BottomCollisionComponent,
            TopCollisionComponent,
            LeftCollisionComponent,
            RightCollisionComponent>()) {
        if (entity->has<PlayerComponent>()) continue; // handled in PlayerSystem
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
    }
}