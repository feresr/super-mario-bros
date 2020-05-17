#include "systems/EnemySystem.h"

void EnemySystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void EnemySystem::tick(World* world) {

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
                enemy->get<TransformComponent>()->h = TILE_SIZE;
                enemy->get<TransformComponent>()->y += TILE_SIZE;
                enemy->remove<AnimationComponent>();
                enemy->remove<WalkComponent>();
                enemy->assign<TextureComponent>(TextureId::TURTLE_SHELL_1);
                enemy->assign<CallbackComponent>([=]() {
                    enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                              TextureId::TURTLE_SHELL_1,
                                                              TextureId::TURTLE_SHELL_2,
                                                      },
                                                      10,
                                                      false);

                    enemy->assign<CallbackComponent>([=]() {
                        enemy->assign<TextureComponent>(TextureId::TURTLE_1);
                        enemy->get<EnemyComponent>()->type = Enemy::Type::TURTLE;
                        enemy->get<KineticComponent>()->accX = 0.0f;
                        enemy->get<KineticComponent>()->speedX = 0.0f;
                        enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                                  TextureId::TURTLE_1,
                                                                  TextureId::TURTLE_2,
                                                          },
                                                          10,
                                                          false);
                        enemyTransform->h = TILE_SIZE * 2;
                        enemyTransform->y -= TILE_SIZE;
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


    for (auto shell : world->find<EnemyComponent, KineticComponent>()) {
        if (shell->get<EnemyComponent>()->type == Enemy::Type::TURTLE_SHELL) {
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
