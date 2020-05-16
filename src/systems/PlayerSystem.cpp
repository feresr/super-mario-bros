#include "systems/PlayerSystem.h"

int dirX = 0;
bool jump = false;
int left = 0;
int right = 0;
int lookingLeft = 0;

constexpr int RUNNING_TEXTURE_ID = 9 * 13 + 10;
constexpr int RUNNING_ANIMATION_SPEED = 5;
constexpr int STANDING_TEXTURE_ID = 9 * 13 + 9;
constexpr int JUMPING_TEXTURE_ID = 10 * 13 + 9;
constexpr int DRAFTING_TEXTURE_ID = 9 * 13 + 13;

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
            player->assign<AnimationComponent>(
                    std::vector<int>{
                            RUNNING_TEXTURE_ID,
                            RUNNING_TEXTURE_ID + 1,
                            RUNNING_TEXTURE_ID + 2},
                    RUNNING_ANIMATION_SPEED
            );
            break;
        }
        case STANDING:
            player->assign<TextureComponent>(STANDING_TEXTURE_ID);
            break;
        case JUMPING:
            player->assign<TextureComponent>(JUMPING_TEXTURE_ID);
            break;
        case DRIFTING:
            player->assign<TextureComponent>(DRAFTING_TEXTURE_ID);
            break;
    }
    currentState = state;
}

void PlayerSystem::tick(World* world) {
    auto kinetic = player->get<KineticComponent>();
    auto transform = player->get<TransformComponent>();

    // Avoid walking back
    if (transform->left() < camera->left()) {
        transform->setLeft(camera->left());
        kinetic->accX = 0;
        kinetic->speedX = 0;
    }

    if (player->has<BottomCollisionComponent>()) {
        kinetic->accX = (float) dirX * MARIO_ACCELERATION_X * 1.5f;
        if (jump) player->get<KineticComponent>()->accY = -MARIO_JUMP;
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
            enemy->assign<TextureComponent>(GOOMBA_CRUSHED_TEXTURE);
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
    auto transform = player->get<TransformComponent>();
    auto x = transform->x;
    auto y = transform->y - TILE_SIZE;
    auto w = transform->w;
    auto h = transform->h * 2;
    player->remove<TransformComponent>();
    player->assign<TransformComponent>(x, y, w, h);
    // TODO: Animation
    // player->assign<AnimationComponent>(std::vector<int>{}, 10);
    SDL_Delay(1000);
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
