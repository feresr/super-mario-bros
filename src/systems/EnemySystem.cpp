#include "systems/EnemySystem.h"

void EnemySystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void turtleShellInteractions(World* world, Entity* shell) {
    if (shell->hasAny<LeftCollisionComponent, RightCollisionComponent>()) {
        for (auto other : world->find<EnemyComponent, KineticComponent>()) {
            if (shell == other) continue;
            if (AABBCollision(shell->get<TransformComponent>(), other->get<TransformComponent>())) {
                other->remove<SolidComponent>();
                other->assign<TileComponent>();
                other->get<TextureComponent>()->flipV = true;
                other->get<KineticComponent>()->speedY = -8;
                shell->remove<LeftCollisionComponent>();
                shell->remove<RightCollisionComponent>();
            }
        }

        if (shell->has<LeftCollisionComponent>()) {
            shell->get<KineticComponent>()->accX = 3.0f;
            shell->get<KineticComponent>()->speedX = 3.0f;
        }
        if (shell->has<RightCollisionComponent>()) {
            shell->get<KineticComponent>()->accX = -3.0f;
            shell->get<KineticComponent>()->speedX = -3.0f;
        }
    }
}

void EnemySystem::tick(World* world) {
    for (auto shell : world->find<EnemyComponent, KineticComponent>()) {
        if (shell->get<EnemyComponent>()->type == Enemy::Type::TURTLE_SHELL) {
            turtleShellInteractions(world, shell);
        }
    }

    for (auto enemy : world->find<EnemyComponent, TransformComponent, CrushedComponent>()) {
        auto enemyTransform = enemy->get<TransformComponent>();
        switch (enemy->get<EnemyComponent>()->type) {
            case Enemy::Type::GOOMBA: {
                enemy->clearComponents();
                enemy->assign<TileComponent>();
                enemy->assign<DestroyDelayedComponent>(50);
                enemy->assign<TextureComponent>(TextureId::GOOMBA_CRUSHED);
                enemy->assign<TransformComponent>(*enemyTransform);
            }
                break;
            case Enemy::Type::TURTLE: {
                enemy->get<EnemyComponent>()->type = Enemy::Type::TURTLE_SHELL;
                enemy->remove<AnimationComponent>();
                enemy->remove<WalkComponent>();
                enemy->get<TextureComponent>()->id = TextureId::TURTLE_SHELL_1;
                enemy->get<TextureComponent>()->offSetY = 0;
                enemy->get<TextureComponent>()->h = TILE_SIZE;
                enemy->assign<CallbackComponent>([=]() {
                    enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                              TextureId::TURTLE_SHELL_1,
                                                              TextureId::TURTLE_SHELL_2,
                                                      },
                                                      10,
                                                      false);

                    enemy->assign<CallbackComponent>([=]() {
                        enemy->get<TextureComponent>()->id = TextureId::TURTLE_1;
                        enemy->get<TextureComponent>()->offSetY = -TILE_SIZE;
                        enemy->get<TextureComponent>()->h = TILE_SIZE * 2;
                        enemy->get<TextureComponent>()->flipH = false;
                        enemy->get<EnemyComponent>()->type = Enemy::Type::TURTLE;
                        enemy->get<KineticComponent>()->accX = 0.0f;
                        enemy->get<KineticComponent>()->speedX = 0.0f;
                        enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                                  TextureId::TURTLE_1,
                                                                  TextureId::TURTLE_2,
                                                          },
                                                          10,
                                                          false);
                        enemy->assign<WalkComponent>(-.4f);
                    }, 100);

                }, 200);
            }
                break;
            case Enemy::Type::TURTLE_SHELL: {
                enemy->get<KineticComponent>()->accX = -3.0f;
            }
        }
    }

    //WALKABLE
    auto entities = world->find<WalkComponent, LeftCollisionComponent>();
    for (auto entity : entities) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = std::abs(walkComponent->speed);
        if (entity->has<TextureComponent>() && entity->has<EnemyComponent>()) {
            entity->get<TextureComponent>()->flipH = true;
        }
    }

    for (auto entity : world->find<WalkComponent, RightCollisionComponent>()) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = -std::abs(walkComponent->speed);
        if (entity->has<TextureComponent>() && entity->has<EnemyComponent>()) {
            entity->get<TextureComponent>()->flipH = false;
        }
    }

    for (auto entity : world->find<WalkComponent, KineticComponent>()) {
        entity->get<KineticComponent>()->speedX = entity->get<WalkComponent>()->speed;
    }

    for (auto entity : world->find<EnemyComponent>()) {
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
        entity->remove<CrushedComponent>();
    }
}

void EnemySystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}
