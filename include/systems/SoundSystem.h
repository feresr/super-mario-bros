#pragma once
#include "ecs/ecs.h"
#include <SDL_mixer.h>
#include "SoundManger.h"
#include "Components.h"

class SoundSystem : public System {

public:
    explicit SoundSystem() = default;

    void onAddedToWorld(World* world) override ;

    void tick(World* world) override ;

    void handleEvent(SDL_Event& event) override ;

    void onRemovedFromWorld(World* world) override ;

    ~SoundSystem() override = default;

private:
    SoundManager* soundManager;

};