#include "ecs/ecs.h"

Entity* World::create() {
    auto* e = new Entity();
    entities.push_back(e);
    return e;
}

void World::destroy(Entity* entity) {
    if (!entity) return;
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    delete entity;
}
//TODO: Use unique_ptr to make the fact that WORLD owns the system pointers (and is in charge of clearing them)
void World::registerSystem(System* system) {
    systems.push_back(system);
    system->onAddedToWorld(this);
}
//TODO: Use unique_ptr to make the fact that WORLD owns the system pointers (and is in charge of clearing them)
void World::unregisterSystem(System* system) {
    if (!system) return;
    systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
    system->onRemovedFromWorld(this);
    delete system;
}

void World::tick() {
    for (auto system : systems) system->tick(this);
}

World::~World() {
    for (auto system : systems) {
        system->onRemovedFromWorld(this);
        delete system;
    }
    systems.clear();

    for (auto entity : entities) delete entity;
    entities.clear();
}

void World::handleEvent(SDL_Event& event) {
    for (auto system : systems) system->handleEvent(event);
}

Entity::~Entity() {
    for (auto& component : *components) delete component.second;
    components->clear();
}
