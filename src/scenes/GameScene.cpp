#include "scenes/GameScene.h"

GameScene::GameScene(SDL_Window* window) {
    world = new World();
    world->registerSystem(new SoundSystem());
    world->registerSystem(new RenderSystem(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT));
    world->registerSystem(new PlayerSystem([](){}));
    world->registerSystem(new MapSystem());
    world->registerSystem(new EnemySystem());
    world->registerSystem(new CallbackSystem());
    world->registerSystem(new AnimationSystem());
    world->registerSystem(new ScoreSystem());
    world->registerSystem(new TileSystem());
    world->registerSystem(new PhysicsSystem());
}

void GameScene::update() {
    Scene::update();
    world->tick();
}


