#include "Components.h"
#include "Constants.h"
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

void createMushroom(World* world, Entity* block, bool oneup = false) {
    auto mushroom = world->create();
    if (oneup) {
        mushroom->assign<Engine::TextureComponent>(TextureId::ONE_UP);
    } else {
        mushroom->assign<Engine::TextureComponent>(TextureId::MUSHROOM);
    }
    world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::MUSHROOM_GROW);
    mushroom->get<Engine::TextureComponent>()->w = TILE_SIZE;
    mushroom->get<Engine::TextureComponent>()->offSetX = -4;

    mushroom->assign<CollectibleComponent>();
    mushroom->assign<Engine::TransformComponent>(
            block->get<Engine::TransformComponent>()->left() + 4,
            block->get<Engine::TransformComponent>()->top(),
            TILE_SIZE - 8,
            TILE_SIZE
    );

    mushroom->assign<GrowComponent>();
}

void createCoin(World* world, Entity* block) {
    auto coin = world->create();
    coin->assign<Engine::TextureComponent>(TextureId::COIN_1);

    coin->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::COIN_1, TextureId::COIN_2, TextureId::COIN_3, TextureId::COIN_4
            }, 5);

    coin->assign<Engine::TransformComponent>(
            block->get<Engine::TransformComponent>()->left(),
            block->get<Engine::TransformComponent>()->top(),
            TILE_SIZE,
            TILE_SIZE
    );
    coin->assign<GravityComponent>();
    coin->assign<TileComponent>();
    coin->assign<KineticComponent>(0.0f, -20.0f);
    coin->assign<CallbackComponent>([=]() {
        auto transform = coin->get<Engine::TransformComponent>();
        world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, transform->getCenterX(), transform->y);
        world->create()->assign<AddScoreComponent>(100, true);
        world->destroy(coin);
    }, 20);

    world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::COIN);
}

void TileSystem::tick(World* world) {

    for (auto entity : world->find<BottomCollisionComponent, TileComponent, KineticComponent>()) {
        auto transform = entity->get<Engine::TransformComponent>();
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

    //Growing mushrooms
    for (auto entity : world->find<GrowComponent, Engine::TransformComponent>()) {
        auto grow = entity->get<GrowComponent>();
        if (!grow->finished()) {
            entity->get<Engine::TransformComponent>()->y -= MUSHROOM_GROW_SPEED;
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
        world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::BLOCK_HIT);
        entity->get<BreakableComponent>()->hit = true;
    }

    for (auto entity : world->find<BreakableComponent>()) {
        auto breakable = entity->get<BreakableComponent>();
        if (!breakable->hit) continue;

        if (entity->has<QuestionBlockComponent>()) {
            entity->remove<AnimationComponent>();
            entity->assign<Engine::TextureComponent>(TextureId::QUESTION_BLOCK_OFF);
        }

        if (!breakable->finished()) {
            entity->get<Engine::TransformComponent>()->y += (float) breakable->getHeight();
        } else {
            breakable->reset();
            entity->get<BreakableComponent>()->hit = false;
            if (entity->has<QuestionBlockComponent>()) {
                if (entity->get<QuestionBlockComponent>()->spawn) createMushroom(world, entity);
                if (entity->get<QuestionBlockComponent>()->coin) createCoin(world, entity);
                if (entity->get<QuestionBlockComponent>()->oneup) createMushroom(world, entity, true);
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

    for (auto entity : world->find<TileComponent>()) {
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
    }
}