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



    if (player->has<BottomCollisionComponent>()) {
        kinetic->accX = (float) dirX * MARIO_ACCELERATION_X * 1.5;
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

    for (auto enemy : world->find<EnemyComponent>()) {
        if (enemy->has<TopCollisionComponent>()) {
            enemy->remove<WalkComponent>();
            enemy->remove<KineticComponent>();
            enemy->remove<TopCollisionComponent>();
            enemy->remove<AnimationComponent>();
            enemy->get<TextureComponent>()->id = 11 * 13 + 7;
            player->get<TransformComponent>()->setBottom(enemy->get<TransformComponent>()->getCenterY());
            player->get<KineticComponent>()->accY = -MARIO_BOUNCE;
        }
    }

    if (left || right) lookingLeft = left;
    player->get<TextureComponent>()->flipH = lookingLeft;
    jump = false;
    player->remove<BottomCollisionComponent>();
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
    System::onRemovedFromWorld(world);
    world->destroy(player);
}

void PlayerSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);

    player = world->create();
    player->assign<PlayerComponent>();

    player->assign<GravityComponent>();
    player->assign<SolidComponent>();
    player->assign<KineticComponent>();
    player->assign<TransformComponent>(40, 40, TILE_SIZE, TILE_SIZE);
}
