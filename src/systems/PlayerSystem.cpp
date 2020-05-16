#include "systems/PlayerSystem.h"

int dirX = 0;
bool jump = false;
int left = 0;
int right = 0;
int lookingLeft = 0;

constexpr int RUNNING_ANIMATION_SPEED = 5;

bool AABBCollision(
        TransformComponent* a,
        TransformComponent* b
) {
    return a->x <= b->x + b->w &&
           a->x + a->w >= b->x &&
           a->y <= b->y + b->h &&
           a->y + a->h >= b->y;
}

void PlayerSystem::setAnimation(ANIMATION_STATE state) {
    if (currentState == state) return;
    player->remove<AnimationComponent>();

    switch (state) {
        case RUNNING: {
            if (player->has<SuperMarioComponent>()) {
                player->assign<AnimationComponent>(
                        std::vector<TextureId>{
                                TextureId::SUPER_MARIO_RUN_1,
                                TextureId::SUPER_MARIO_RUN_2,
                                TextureId::SUPER_MARIO_RUN_3},
                        RUNNING_ANIMATION_SPEED
                );
            } else {
                player->assign<AnimationComponent>(
                        std::vector<TextureId>{
                                TextureId::MARIO_RUN_1,
                                TextureId::MARIO_RUN_2,
                                TextureId::MARIO_RUN_3},
                        RUNNING_ANIMATION_SPEED
                );
            }

            break;
        }
        case STANDING:
            player->assign<TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_STAND :
                    TextureId::MARIO_STAND);
            break;
        case JUMPING:
            player->assign<TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_JUMP :
                    TextureId::MARIO_JUMP);
            break;
        case DRIFTING:
            player->assign<TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_DRIFT :
                    TextureId::MARIO_DRIFT);
            break;
    }
    currentState = state;
}

void PlayerSystem::tick(World* world) {
    auto transform = player->get<TransformComponent>();

    if (player->has<FrozenComponent>()) { // Mario Growing after eating mushroom
        if ((player->get<TextureComponent>()->id == TextureId::SUPER_MARIO_STAND ||
             player->get<TextureComponent>()->id == TextureId::MARIO_GROWING)
            && transform->h <= TILE_SIZE) {
            transform->h = TILE_SIZE * 2;
            transform->y -= TILE_SIZE;
        }
        if (player->get<TextureComponent>()->id == TextureId::MARIO_STAND
            && transform->h > TILE_SIZE) {
            transform->h = TILE_SIZE;
            transform->y += TILE_SIZE;
        }
        return;
    }

    auto kinetic = player->get<KineticComponent>();

    // Avoid walking back
    if (transform->left() < camera->left()) {
        transform->setLeft(camera->left());
        kinetic->accX = 0;
        kinetic->speedX = 0;
    }

    if (player->has<BottomCollisionComponent>()) {
        kinetic->accX = (float) dirX * MARIO_ACCELERATION_X * 1.5f;
        if (jump) player->get<KineticComponent>()->accY = -MARIO_JUMP_ACCELERATION;
        if ((bool) std::abs(kinetic->speedX) || (bool) std::abs(kinetic->accX)) {
            if ((kinetic->speedX > 0 && kinetic->accX < 0) ||
                (kinetic->speedX < 0 && kinetic->accX > 0)) {
                setAnimation(DRIFTING);
            } else {
                setAnimation(RUNNING);
            }
        } else {
            setAnimation(STANDING);
        }
    } else {
        kinetic->accX = (float) (dirX) * (MARIO_ACCELERATION_X);
        setAnimation(JUMPING);
    }

    // Step on enemies
    for (auto enemy : world->find<EnemyComponent, TransformComponent>()) {
        if (!AABBCollision(enemy->get<TransformComponent>(), transform)) continue;
        if (enemy->has<TopCollisionComponent>()) {
            auto enemyTransform = enemy->get<TransformComponent>();
            transform->setBottom(enemyTransform->getCenterY());
            enemy->clearComponents();
            enemy->assign<TileComponent>();
            enemy->assign<DestroyDelayedComponent>(50);
            enemy->assign<TextureComponent>(TextureId::GOOMBA_CRUSHED);
            enemy->assign<TransformComponent>(*enemyTransform);
            kinetic->accY = -MARIO_BOUNCE;
            kinetic->speedY = 0;
        } else {
            player->remove<SuperMarioComponent>();
            transform->h = TILE_SIZE;
        }
    }

    // Break bricks
    if (player->has<SuperMarioComponent>()) {
        for (auto breakable : world->find<BreakableComponent, TransformComponent, BottomCollisionComponent>()) {
            if (!breakable->has<QuestionBlockComponent>() &&
                AABBCollision(breakable->get<TransformComponent>(), transform)) {
                breakable->clearComponents();
            }
        }
    }

    // Eat mushrooms
    for (auto collectible : world->find<CollectibleComponent, TransformComponent>()) {
        if (AABBCollision(collectible->get<TransformComponent>(), player->get<TransformComponent>())) {
            eatMushroom();
            world->destroy(collectible);
        }
    }

    if (left || right) lookingLeft = left;
    player->get<TextureComponent>()->flipH = lookingLeft;
    jump = false;
    player->remove<BottomCollisionComponent>();
    player->remove<LeftCollisionComponent>();
    player->remove<RightCollisionComponent>();
    player->remove<TopCollisionComponent>();
}

void PlayerSystem::eatMushroom() {
    if (player->has<SuperMarioComponent>()) return;
    player->assign<SuperMarioComponent>();
    player->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::SUPER_MARIO_STAND
            }, 8, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<CallbackComponent>([&]() { player->remove<FrozenComponent>(); }, 80);
}

void PlayerSystem::handleEvent(SDL_Event& event) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) return;

    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                    left = true;
                    break;
                case SDL_SCANCODE_D:
                    right = true;
                    break;
                case SDL_SCANCODE_W:
                    jump = true;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                    left = false;
                    break;
                case SDL_SCANCODE_D:
                    right = false;
                    break;
                default:
                    break;
            }
    }

    dirX = right - left;
}

void PlayerSystem::onRemovedFromWorld(World* world) {
    camera = nullptr;
    System::onRemovedFromWorld(world);
    world->destroy(player);
}

void PlayerSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);

    camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    player = world->create();
    player->assign<PlayerComponent>();

    player->assign<GravityComponent>();
    player->assign<SolidComponent>();
    player->assign<KineticComponent>();
    player->assign<TransformComponent>(40, 40, TILE_SIZE, TILE_SIZE);
}
