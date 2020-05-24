#include "scenes/IntroScene.h"

IntroScene::IntroScene(SDL_Window* window) {
    world = new World();
    auto renderSystem = new RenderSystem(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT);
    renderSystem->setBackgroundColor(0, 0, 0);
    world->registerSystem(renderSystem);
    world->registerSystem(new ScoreSystem());

    auto worldName = world->create();
    worldName->assign<TextComponent>("WORLD 1-1");
    auto width = 72;
    worldName->assign<TransformComponent>(SNES_RESOLUTION_WIDTH / 2 - width / 2,
                                          SNES_RESOLUTION_HEIGHT / 2 - 30, width,
                                          8);

    auto mario = world->create();
    mario->assign<TextureComponent>(TextureId::MARIO_STAND);
    mario->assign<TextComponent>("");
    mario->assign<TransformComponent>(SNES_RESOLUTION_WIDTH / 2 - TILE_SIZE / 2 - 20, SNES_RESOLUTION_HEIGHT / 2,
                                      TILE_SIZE,
                                      TILE_SIZE);

    auto x = world->create();
    x->assign<TextComponent>("x");
    x->assign<TransformComponent>(SNES_RESOLUTION_WIDTH / 2 - 4, SNES_RESOLUTION_HEIGHT / 2 + 6, 8, 8);

    auto three = world->create();
    three->assign<TextComponent>("3");
    three->assign<TransformComponent>(SNES_RESOLUTION_WIDTH / 2 - 4 + 20, SNES_RESOLUTION_HEIGHT / 2 + 6, 8, 8);
}

void IntroScene::update() {
    Scene::update();
    world->tick();
}


