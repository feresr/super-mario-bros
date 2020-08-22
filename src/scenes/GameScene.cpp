#include "scenes/GameScene.h"

GameScene::GameScene(SDL_Window* window) {
    world->registerSystem<SoundSystem>();
    world->registerSystem<RenderSystem>(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT);
    world->registerSystem<PlayerSystem>([&](){ this->gameOver = true; });
    world->registerSystem<MapSystem>();
    world->registerSystem<EnemySystem>();
    world->registerSystem<CallbackSystem>();
    world->registerSystem<AnimationSystem>();
    world->registerSystem<ScoreSystem>();
    world->registerSystem<FlagSystem>(([&](){ this->gameOver = true; }));
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();
}

bool GameScene::isFinished() {
    return gameOver;
}


