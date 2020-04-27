#include "ecs/ecs.h"

Entity* World::create() {
    auto* e = new Entity();
    entities.push_back(e);
    return e;
}

void World::destroy(Entity* entity) {
    if (!entity) return;
    entities.erase(std::remove(entities.begin(), entities.end(), entity));
    delete entity;
}

void World::registerSystem(System* system) {
    systems.push_back(system);
    system->onAddedToWorld(this);
}

void World::unregisterSystem(System* system) {
    systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
    system->onRemovedFromWorld(this);
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

Entity::~Entity() {
    for (auto component : components) delete component.second;
    components.clear();
}
