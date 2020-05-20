#include "systems/PlayerSystem.h"

int dirX = 0;
bool jump = false;
bool duck = false;
int left = 0;
int right = 0;
int lookingLeft = 0;

const int SMALL_MARIO_COLLIDER_HEIGHT = TILE_SIZE - 2;
const int SUPER_MARIO_COLLIDER_HEIGHT = (TILE_SIZE * 2) - 4;

constexpr int RUNNING_ANIMATION_SPEED = 4;

void createDebris(World* world, TransformComponent* brickTransform) {
    auto debris1 = world->create();
    debris1->assign<GravityComponent>();
    debris1->assign<KineticComponent>(-2.0f, -16.0f);
    debris1->assign<TileComponent>();
    debris1->assign<TextureComponent>(TextureId::BRICK_DEBRIS_1);
    debris1->assign<TransformComponent>(brickTransform->x, brickTransform->y, 8, 8);

    auto debris2 = world->create();
    debris2->assign<GravityComponent>();
    debris2->assign<KineticComponent>(2.0f, -16.0f);
    debris2->assign<TileComponent>();
    debris2->assign<TextureComponent>(TextureId::BRICK_DEBRIS_2);
    debris2->assign<TransformComponent>(brickTransform->x + 8, brickTransform->y, 8, 8);

    auto debris3 = world->create();
    debris3->assign<GravityComponent>();
    debris3->assign<KineticComponent>(-2.0f, -10.0f);
    debris3->assign<TileComponent>();
    debris3->assign<TextureComponent>(TextureId::BRICK_DEBRIS_3);
    debris3->assign<TransformComponent>(brickTransform->x, brickTransform->y + 8, 8, 8);

    auto debris4 = world->create();
    debris4->assign<GravityComponent>();
    debris4->assign<KineticComponent>(2.0f, -10.0f);
    debris4->assign<TileComponent>();
    debris4->assign<TextureComponent>(TextureId::BRICK_DEBRIS_4);
    debris4->assign<TransformComponent>(brickTransform->x + 8, brickTransform->y + 8, 8, 8);

    world->create()->assign<SoundComponent>(Sound::Id::BLOCK_BREAK);
}

void onGameOver(World* world, Entity* player) {
    world->create()->assign<SoundComponent>(Sound::Id::DEATH);
    auto music = world->findFirst<MusicComponent>();
    if (music) world->destroy(music);

    auto kinetic = player->get<KineticComponent>();
    player->get<TextureComponent>()->id = TextureId::MARIO_DEAD;
    player->assign<DeadComponent>();

    player->remove<AnimationComponent>();
    kinetic->speedY = 0.0f;
    kinetic->speedX = 0.0f;
    kinetic->accY = 0.0f;
    kinetic->accX = 0.0f;

    for (auto walkable : world->findAny<WalkComponent, KineticComponent>()) {
        if (walkable == player) continue;
        walkable->remove<WalkComponent>();
        walkable->remove<KineticComponent>();
        walkable->remove<AnimationComponent>();
        walkable->remove<CallbackComponent>();
    }

    player->assign<CallbackComponent>([=]() {
        player->remove<SolidComponent>();
        kinetic->speedY = -10.0f;
        kinetic->speedX = 0.0f;
        kinetic->accY = 0.0f;
        kinetic->accX = 0.0f;
    }, 50);
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
        case DUCKING:
            if (player->has<SuperMarioComponent>()) {
                player->assign<TextureComponent>(TextureId::SUPER_MARIO_DUCK);
            }
    }

    if (player->has<SuperMarioComponent>()) {
        player->get<TextureComponent>()->h = TILE_SIZE * 2;
        player->get<TextureComponent>()->offSetY = -3;
    } else {
        player->get<TextureComponent>()->h = TILE_SIZE;
        player->get<TextureComponent>()->offSetY = -1;
    }
    player->get<TextureComponent>()->w = TILE_SIZE;
    player->get<TextureComponent>()->offSetX = -2;
    currentState = state;
}

void PlayerSystem::tick(World* world) {
    if (player->has<DeadComponent>()) return;

    auto transform = player->get<TransformComponent>();

    if (player->has<FrozenComponent>()) { // Mario Growing after eating mushroom
        if ((player->get<TextureComponent>()->id == TextureId::SUPER_MARIO_STAND ||
             player->get<TextureComponent>()->id == TextureId::MARIO_GROWING)
            && transform->h <= TILE_SIZE) {
            player->get<TextureComponent>()->h = TILE_SIZE * 2;
            transform->h = SUPER_MARIO_COLLIDER_HEIGHT;
            transform->y -= TILE_SIZE;
        }
        if (player->get<TextureComponent>()->id == TextureId::MARIO_STAND
            && transform->h > TILE_SIZE) {
            player->get<TextureComponent>()->h = TILE_SIZE;
            transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
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

    if (duck && player->has<SuperMarioComponent>()) {
        setAnimation(DUCKING);
        kinetic->accX = 0;
    } else {
        if (player->has<BottomCollisionComponent>()) {
            kinetic->accX = (float) dirX * MARIO_ACCELERATION_X * 1.7f;
            if (jump) {
                player->get<KineticComponent>()->accY = -MARIO_JUMP_ACCELERATION;
                auto jumpsound = world->create();
                jumpsound->assign<SoundComponent>(Sound::Id::JUMP);
            }
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
    }

    for (auto enemy : world->find<EnemyComponent, TransformComponent>()) {
        if (!AABBCollision(enemy->get<TransformComponent>(), transform)) continue;
        if (enemy->has<TopCollisionComponent>()) {
            enemy->assign<CrushedComponent>();
            transform->setBottom(enemy->get<TransformComponent>()->top());
            kinetic->accY = -0.2f;
            kinetic->speedY = -MARIO_BOUNCE;
        } else {
            if (player->has<SuperMarioComponent>()) {
                world->create()->assign<SoundComponent>(Sound::Id::SHRINK);
                player->remove<SuperMarioComponent>();
                transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
            } else {
                onGameOver(world, player);
                return;
            }
        }
    }

    // Break bricks
    if (player->has<SuperMarioComponent>()) {
        for (auto breakable : world->find<BreakableComponent, TransformComponent, BottomCollisionComponent>()) {
            if (!breakable->has<QuestionBlockComponent>() &&
                AABBCollision(breakable->get<TransformComponent>(), transform)) {
                createDebris(world, breakable->get<TransformComponent>());
                breakable->clearComponents();
            }
        }
    }

    // Eat mushrooms
    for (auto collectible : world->find<CollectibleComponent, TransformComponent>()) {
        if (AABBCollision(collectible->get<TransformComponent>(), player->get<TransformComponent>())) {
            eatMushroom(world);
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

void PlayerSystem::eatMushroom(World* world) {
    if (player->has<SuperMarioComponent>()) return;
    world->create()->assign<SoundComponent>(Sound::Id::MUSHROOM_EAT);
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
                case SDL_SCANCODE_S:
                    duck = true;
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
                case SDL_SCANCODE_S:
                    duck = false;
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
    player->assign<TransformComponent>(40, 40, TILE_SIZE - 4, SMALL_MARIO_COLLIDER_HEIGHT);
}
