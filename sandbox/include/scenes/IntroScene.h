#pragma once

#include "Scene.h"
#include <systems/RenderSystem.h>
#include <systems/ScoreSystem.h>

class IntroScene : public Scene {

public:
    explicit IntroScene(SDL_Window* window);

    bool isFinished() override;

    void update() override;

private:
    int timer = 0;
};