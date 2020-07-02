#include "scenes/Scene.h"

class IntroScene : public Scene {

public:
    IntroScene(SDL_Window* window);

private:
    int timer = 0;
    void update() override;

public:
    bool isFinished() override;

};