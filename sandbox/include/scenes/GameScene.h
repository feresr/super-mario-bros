#pragma once

#include "Scene.h"

class GameScene : public Scene {

public:
    explicit GameScene(SDL_Window* window);

    bool isFinished() override;

private:
    bool gameOver = false;

};