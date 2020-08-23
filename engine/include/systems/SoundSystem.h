#pragma once

#include "ecs.h"
#include "SoundManger.h"

namespace Engine {
    class SoundSystem : public System {

    public:
        explicit SoundSystem() = default;

        void onAddedToWorld(World* world) override;

        void tick(World* world) override;

        void handleEvent(SDL_Event& event) override;

        void onRemovedFromWorld(World* world) override;

        ~SoundSystem() override = default;

    private:
        Engine::SoundManager* soundManager{};
    };

}
