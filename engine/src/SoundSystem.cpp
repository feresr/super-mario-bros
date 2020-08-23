#include "systems/SoundSystem.h"
#include "Components.h"

namespace Engine {
    void SoundSystem::onAddedToWorld(World* world) {
        System::onAddedToWorld(world);
        soundManager = new Engine::SoundManager();
        world->create()->assign<Engine::MusicComponent>(Engine::Music::Id::BACKGROUND);
    }

    void SoundSystem::tick(World* world) {
        auto music = world->findFirst<Engine::MusicComponent>();
        if (music) soundManager->playMusic(music->get<Engine::MusicComponent>()->music);

        for (auto sound : world->find<Engine::SoundComponent>()) {
            if (sound->get<Engine::SoundComponent>()->sound == Engine::Sound::DEATH) soundManager->stopMusic();
            soundManager->playSound(sound->get<Engine::SoundComponent>()->sound);
            world->destroy(sound);
        }
        world->destroy(music);
    }

    void SoundSystem::handleEvent(SDL_Event& event) {
        System::handleEvent(event);
    }

    void SoundSystem::onRemovedFromWorld(World* world) {
        System::onRemovedFromWorld(world);
        delete soundManager;
    }
}
