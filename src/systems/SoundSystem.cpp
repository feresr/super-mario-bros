#include "systems/SoundSystem.h"

void SoundSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
    soundManager = new SoundManager();
}

void SoundSystem::tick(World* world) {

}

void SoundSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void SoundSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
    delete soundManager;
}
