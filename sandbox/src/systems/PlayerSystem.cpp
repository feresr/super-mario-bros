#include <Constants.h>
#include <iostream>
#include <ResourceManager.h>
#include <SDL_timer.h>
#include "systems/PlayerSystem.h"
#include "GameComponents.h"

int dirX = 0;
bool jump = false;
bool duck = false;
bool sprint = false;
int left = 0;
int right = 0;
int lookingLeft = 0;

const int SMALL_MARIO_COLLIDER_HEIGHT = TILE_SIZE - 2;
const int SUPER_MARIO_COLLIDER_HEIGHT = (TILE_SIZE * 2) - 4;

constexpr int RUNNING_ANIMATION_SPEED = 4;

void shrink(World* world, Entity* player) {
    world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::SHRINK);
    player->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND
            }, 8, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<BlinkingComponent>(10);
    player->remove<SuperMarioComponent>();

    player->assign<CallbackComponent>([player]() {
        player->remove<FrozenComponent>();
        player->assign<CallbackComponent>([player]() { player->remove<BlinkingComponent>(); }, 150);
    }, 64);
}

void createDebris(World* world, Engine::TransformComponent* brickTransform) {
    auto debris1 = world->create();
    debris1->assign<GravityComponent>();
    debris1->assign<KineticComponent>(-2.0f, -16.0f);
    debris1->assign<TileComponent>();
    debris1->assign<Engine::TextureComponent>(TextureId::BRICK_DEBRIS_1);
    debris1->assign<Engine::TransformComponent>(brickTransform->x, brickTransform->y, 8, 8);

    auto debris2 = world->create();
    debris2->assign<GravityComponent>();
    debris2->assign<KineticComponent>(2.0f, -16.0f);
    debris2->assign<TileComponent>();
    debris2->assign<Engine::TextureComponent>(TextureId::BRICK_DEBRIS_2);
    debris2->assign<Engine::TransformComponent>(brickTransform->x + 8, brickTransform->y, 8, 8);

    auto debris3 = world->create();
    debris3->assign<GravityComponent>();
    debris3->assign<KineticComponent>(-2.0f, -10.0f);
    debris3->assign<TileComponent>();
    debris3->assign<Engine::TextureComponent>(TextureId::BRICK_DEBRIS_3);
    debris3->assign<Engine::TransformComponent>(brickTransform->x, brickTransform->y + 8, 8, 8);

    auto debris4 = world->create();
    debris4->assign<GravityComponent>();
    debris4->assign<KineticComponent>(2.0f, -10.0f);
    debris4->assign<TileComponent>();
    debris4->assign<Engine::TextureComponent>(TextureId::BRICK_DEBRIS_4);
    debris4->assign<Engine::TransformComponent>(brickTransform->x + 8, brickTransform->y + 8, 8, 8);

    world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::BLOCK_BREAK);
}

void PlayerSystem::onGameOver(World* world, Entity* player) {
    if (player->has<SuperMarioComponent>()) {
        //player time ran out while being Super Mario
        player->remove<SuperMarioComponent>();
        auto texture = player->get<Engine::TextureComponent>();
        auto transform = player->get<Engine::TransformComponent>();
        transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
        transform->y += SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
        texture->h = TILE_SIZE;
        texture->offSetY = -1;
    }
    player->assign<DeadComponent>();
    world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::DEATH);
    auto music = world->findFirst<Engine::MusicComponent>();
    if (music) world->destroy(music);

    auto kinetic = player->get<KineticComponent>();
    player->get<Engine::TextureComponent>()->id = TextureId::MARIO_DEAD;
    player->assign<DeadComponent>();

    player->remove<AnimationComponent>();
    kinetic->speedY = 0.0f;
    kinetic->speedX = 0.0f;
    kinetic->accY = 0.0f;
    kinetic->accX = 0.0f;

    world->findAny<WalkComponent, KineticComponent>([=](Entity* walker) {
        if (walker != player) {
            walker->remove<WalkComponent>();
            walker->remove<KineticComponent>();
            walker->remove<AnimationComponent>();
            walker->remove<CallbackComponent>();
        }
    });

    player->assign<CallbackComponent>([=]() {
        player->remove<SolidComponent>();
        kinetic->speedY = -10.0f;
        kinetic->speedX = 0.0f;
        kinetic->accY = 0.0f;
        kinetic->accX = 0.0f;

        player->assign<CallbackComponent>([&]() { gameOverCallback(); }, 200);
    }, 50);
}

void PlayerSystem::setAnimation(ANIMATION_STATE state) {
    if (player->has<BlinkingComponent>()) {
        auto blink = player->get<BlinkingComponent>();
        blink->current++;
        if ((blink->current / blink->speed) % 2) {
            player->remove<AnimationComponent>();
            currentState = BLINKING;
            player->get<Engine::TextureComponent>()->id = TextureId::EMPTY;
            return;
        }
    }
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
            player->assign<Engine::TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_STAND :
                    TextureId::MARIO_STAND);
            break;
        case JUMPING:
            player->assign<Engine::TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_JUMP :
                    TextureId::MARIO_JUMP);
            break;
        case DRIFTING:
            player->assign<Engine::TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_DRIFT :
                    TextureId::MARIO_DRIFT);
            break;
        case DUCKING:
            if (player->has<SuperMarioComponent>()) {
                player->assign<Engine::TextureComponent>(TextureId::SUPER_MARIO_DUCK);
            }
            break;
        case BLINKING:
            break;
    }

    if (player->has<SuperMarioComponent>()) {
        player->get<Engine::TextureComponent>()->h = TILE_SIZE * 2;
        player->get<Engine::TextureComponent>()->offSetY = -3;
    } else {
        player->get<Engine::TextureComponent>()->h = TILE_SIZE;
        player->get<Engine::TextureComponent>()->offSetY = -1;
    }
    player->get<Engine::TextureComponent>()->w = TILE_SIZE;
    player->get<Engine::TextureComponent>()->offSetX = -2;
    currentState = state;
}

void PlayerSystem::tick(World* world) {

    auto midpointX = SNES_RESOLUTION_WIDTH / 2.0f;
    auto midpointY = (SNES_RESOLUTION_HEIGHT - 50) / 2.0f;


    auto cameraPosition = glm::vec3{midpointX + cos((SDL_GetTicks() * .001f)) * 80.0f,
                                    midpointY + sin((SDL_GetTicks() * .001f)) * 50.0f, -380.3f};
    //auto cameraPosition = glm::vec3{0.0f, 0.0f, 270.3f};
    glm::mat4 view = glm::lookAt(
            cameraPosition,
            glm::vec3{256.0f / 2, 224.0f / 2, 0.0f}, //glm::vec3{256.0f / 2.0f, 224.0f / 2.0f, 0.0f} ,
            glm::vec3(0.0f, 01.0f, 0.0f)
    );

    glm::mat4 projection = glm::perspective(glm::radians(-45.0f),
                                            static_cast<float>(256) / static_cast<float>(224),
                                            200.0f, 800.0f
    );


    auto shader = ResourceManager::GetShader("shader")
            .Use()
            .SetInteger("image", 0)
            .SetMatrix4("view", view)
            .SetMatrix4("projection", projection);


    if (world->findFirst<GameOverComponent>()) {
        world->findFirst<GameOverComponent>()->remove<GameOverComponent>();
        onGameOver(world, player);
    }
    if (player->has<DeadComponent>()) return;

    auto transform = player->get<Engine::TransformComponent>();
    auto texture = player->get<Engine::TextureComponent>();

    if (player->has<FrozenComponent>()) {
        if (texture) {
            if ((texture->id == TextureId::SUPER_MARIO_STAND ||
                 player->get<Engine::TextureComponent>()->id == TextureId::MARIO_GROWING) &&
                transform->h != SUPER_MARIO_COLLIDER_HEIGHT) {
                texture->h = TILE_SIZE * 2;
                transform->h = SUPER_MARIO_COLLIDER_HEIGHT;
                transform->y -= SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
                texture->offSetY = -3;
            }
            if (texture->id == TextureId::MARIO_STAND
                && transform->h != SMALL_MARIO_COLLIDER_HEIGHT) {
                texture->h = TILE_SIZE;
                texture->offSetY = -1;
                transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
                transform->y += SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
            }
        }
        return;
    }

    auto kinetic = player->get<KineticComponent>();

    // Avoid walking back
    if (transform->left() < camera->left()) {
        transform->setLeft(camera->left());
        kinetic->accX = 0;
        kinetic->accX = 0;
        kinetic->speedX = 0;
    }

    if (duck && player->has<SuperMarioComponent>()) {
        setAnimation(DUCKING);
        kinetic->accX = 0;
    } else {
        if (player->has<BottomCollisionComponent>()) {
            kinetic->accX = (float) dirX * MARIO_ACCELERATION_X * 1.7f;
            if (sprint) kinetic->accX *= 1.5;
            if (jump) {
                player->get<KineticComponent>()->accY = -MARIO_JUMP_ACCELERATION;
                world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::JUMP);
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
            if (sprint) kinetic->accX *= 1.5;
            setAnimation(JUMPING);
        }
    }

    auto bounce = false;
    for (auto enemy : world->find<EnemyComponent, Engine::TransformComponent>()) {
        if (!AABBCollision(enemy->get<Engine::TransformComponent>(), transform)) continue;
        if (kinetic->speedY > 0) {
            enemy->assign<CrushedComponent>();
            transform->setBottom(enemy->get<Engine::TransformComponent>()->top());
            bounce = true;
        } else {
            if (enemy->get<EnemyComponent>()->type == Enemy::TURTLE_SHELL) continue; // ?
            if (!player->has<BlinkingComponent>()) {
                if (player->has<SuperMarioComponent>()) {
                    currentState = ANIMATION_STATE::STANDING;
                    shrink(world, player);
                } else {
                    onGameOver(world, player);
                }
                return;
            }
        }
    }

    if (bounce) {
        kinetic->accY = -0.2f;
        kinetic->speedY = -MARIO_BOUNCE;
    }

    // Break bricks
    if (player->has<SuperMarioComponent>()) {
        for (auto breakable : world->find<BreakableComponent, Engine::TransformComponent, BottomCollisionComponent>()) {
            if (!breakable->has<QuestionBlockComponent>() &&
                AABBCollision(breakable->get<Engine::TransformComponent>(), transform)) {
                createDebris(world, breakable->get<Engine::TransformComponent>());
                breakable->remove<SolidComponent>();
                breakable->remove<Engine::TextureComponent>();
                breakable->assign<BreakableComponent>();
                breakable->get<BreakableComponent>()->hit = true;
            }
        }
    }

    // Eat mushrooms
    for (auto collectible : world->find<CollectibleComponent, Engine::TransformComponent>()) {
        if (AABBCollision(collectible->get<Engine::TransformComponent>(), player->get<Engine::TransformComponent>())) {
            currentState = ANIMATION_STATE::STANDING;
            eatMushroom(world, collectible->get<Engine::TextureComponent>()->id == TextureId::ONE_UP);
            world->destroy(collectible);
        }
    }

    if (left || right) lookingLeft = left;
    player->get<Engine::TextureComponent>()->flipH = lookingLeft;
    jump = false;
    player->remove<BottomCollisionComponent>();
    player->remove<LeftCollisionComponent>();
    player->remove<RightCollisionComponent>();
    player->remove<TopCollisionComponent>();
}

void PlayerSystem::eatMushroom(World* world, bool oneup) {
    if (oneup) {
        world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::ONE_UP);
        world->create()->assign<FloatingPointsComponent>(Points::ONEUP,
                                                         player->get<Engine::TransformComponent>()->getCenterX(),
                                                         player->get<Engine::TransformComponent>()->y
        );
        return;
    } else {
        world->create()->assign<Engine::SoundComponent>(Engine::Sound::Id::MUSHROOM_EAT);
        world->create()->assign<FloatingPointsComponent>(Points::ONETHOUSAND,
                                                         player->get<Engine::TransformComponent>()->getCenterX(),
                                                         player->get<Engine::TransformComponent>()->y
        );
        world->create()->assign<AddScoreComponent>(1000);
    }

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
            }, 6, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<CallbackComponent>([&]() {
        player->remove<FrozenComponent>();
    }, 60);
}

void PlayerSystem::handleEvent(SDL_Event& event) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) return;

    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                    left = true;
                    break;
                case SDL_SCANCODE_LSHIFT:
                    sprint = true;
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
                case SDL_SCANCODE_LSHIFT:
                    sprint = false;
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

    camera = world->findFirst<Engine::CameraComponent>()->get<Engine::CameraComponent>();
    player = world->create();
    player->assign<PlayerComponent>();

    player->assign<GravityComponent>();
    player->assign<SolidComponent>();
    player->assign<KineticComponent>();
    player->assign<Engine::TransformComponent>(40, 140, TILE_SIZE - 4, SMALL_MARIO_COLLIDER_HEIGHT, -30.0f);
}
