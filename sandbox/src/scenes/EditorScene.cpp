#include <systems/EditorSystem.h>
#include <systems/TileSystem.h>
#include <systems/PhysicsSystem.h>
#include <systems/RenderSystem.h>
#include "EditorScene.h"

using namespace Engine;
EditorScene::EditorScene(SDL_Window* window) {
    world->registerSystem<RenderSystem>(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT);
    world->registerSystem<EditorSystem>();
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();

    auto title = world->create();
    title->assign<TextComponent>("MAP EDITOR MODE");
    title->assign<TransformComponent>(10, 10, 60, 8);

    auto instructions1 = world->create();
    instructions1->assign<TextComponent>("- Press up/down keys to change tiles");
    instructions1->assign<TransformComponent>(10, 20, 92, 5);

    auto instructions2 = world->create();
    instructions2->assign<TextComponent>("- Press keys 'a' and 'd' to pan camera");
    instructions2->assign<TransformComponent>(10, 26, 96, 5);

    auto instructions3 = world->create();
    instructions3->assign<TextComponent>("- Press 's' to save the map to disk");
    instructions3->assign<TransformComponent>(10, 32, 90, 5);

    auto instructions4 = world->create();
    instructions4->assign<TextComponent>("- Click to add and remove tiles");
    instructions4->assign<TransformComponent>(10, 38, 80, 5);

    auto instructions5 = world->create();
    instructions5->assign<TextComponent>("- Press 'e' to exit the editor");
    instructions5->assign<TransformComponent>(10, 44, 80, 5);
}

bool EditorScene::isFinished() {
    return false;
}


