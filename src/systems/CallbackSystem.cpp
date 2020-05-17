#include "systems/CallbackSystem.h"

void CallbackSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void CallbackSystem::tick(World* world) {
    for (auto entities : world->find<CallbackComponent>()) {
        auto callback = entities->get<CallbackComponent>();
        if (callback->time <= 0) {
            entities->remove<CallbackComponent>();
            continue;
        }
        callback->time--;
        if (callback->time <= 0) callback->callback();
    }
}

void CallbackSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void CallbackSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}
