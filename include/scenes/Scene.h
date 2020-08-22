#pragma once

#include "systems/EditorSystem.h"
#include "systems/RenderSystem.h"
#include "systems/EditorSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/CallbackSystem.h"
#include "systems/EnemySystem.h"
#include "systems/TileSystem.h"
#include "systems/MapSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/PlayerSystem.h"
#include "systems/SoundSystem.h"
#include "systems/FlagSystem.h"
#include "systems/ScoreSystem.h"
#include "Constants.h"

class Scene {
protected:
    World* world = new World();

public:

    virtual void update() {
        world->tick();
    };

    virtual bool isFinished() { return true; }
    virtual void handleEvents(SDL_Event& event) {
        world->handleEvent(event);
    };

    virtual ~Scene() {
        std::cout << "Scene deleted" << std::endl;
        delete world;
    };
};