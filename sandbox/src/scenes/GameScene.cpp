#include <systems/SoundSystem.h>
#include <systems/RenderSystem.h>
#include <systems/PlayerSystem.h>
#include <systems/MapSystem.h>
#include <systems/AnimationSystem.h>
#include <systems/CallbackSystem.h>
#include <systems/EnemySystem.h>
#include <systems/ScoreSystem.h>
#include <systems/FlagSystem.h>
#include <systems/TileSystem.h>
#include <systems/PhysicsSystem.h>
#include "scenes/GameScene.h"

GameScene::GameScene(SDL_Window* window) {
    world->registerSystem<Engine::SoundSystem>();
    world->registerSystem<Engine::RenderSystem>(window,
                                                SNES_RESOLUTION_WIDTH,
                                                SNES_RESOLUTION_HEIGHT,
                                                glm::vec3{SKY_RED, SKY_GREEN, SKY_BLUE});
    world->registerSystem<PlayerSystem>([&]() { this->gameOver = true; });
    world->registerSystem<MapSystem>();
    world->registerSystem<EnemySystem>();
    world->registerSystem<CallbackSystem>();
    world->registerSystem<AnimationSystem>();
    world->registerSystem<ScoreSystem>();
    world->registerSystem<FlagSystem>(([&]() { this->gameOver = true; }));
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();
}

bool GameScene::isFinished() {
    return gameOver;
}


