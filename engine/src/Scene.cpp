#include "Scene.h"
#include <iostream>

void Scene::update() {
    world->tick();
}


Scene::~Scene() {
    std::cout << "Scene deleted" << std::endl;
    delete world;
}

bool Scene::isFinished() {
    return true;
}

void Scene::handleEvents(SDL_Event& event) {
    world->handleEvent(event);
}
