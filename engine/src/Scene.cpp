#include "Scene.h"
#include <Log.h>

void Scene::update() {
    world->tick();
}


Scene::~Scene() {
    ENGINE_INFO("Scene deleted");
    delete world;
}

bool Scene::isFinished() {
    return true;
}

void Scene::handleEvents(SDL_Event& event) {
    world->handleEvent(event);
}
