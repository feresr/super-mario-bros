#include "systems/SoundSystem.h"

void SoundSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
    soundManager = new SoundManager();
    world->create()->assign<MusicComponent>();
}

void SoundSystem::tick(World* world) {
    auto music = world->findFirst<MusicComponent>();
    if (music) soundManager->playMusic(); else soundManager->stopMusic();

    for (auto sound : world->find<SoundComponent>()) {
        soundManager->playSound(sound->get<SoundComponent>()->sound);
        world->destroy(sound);
    }
}

void SoundSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void SoundSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
    delete soundManager;
}
