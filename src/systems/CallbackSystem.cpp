#include "systems/CallbackSystem.h"

void CallbackSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void CallbackSystem::tick(World* world) {
    for (auto entities : world->find<CallbackComponent>()) {
        auto callback = entities->get<CallbackComponent>();
        callback->time--;
        if (callback->time <= 0) {
            callback->callback();
            entities->remove<CallbackComponent>();
        }
    }
}

void CallbackSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void CallbackSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}
