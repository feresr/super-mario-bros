#ifndef INTROSCENE_H_INCLUDED
#define INTROSCENE_H_INCLUDED

#include "scenes/Scene.h"

class IntroScene : public Scene {

public:
    explicit IntroScene(SDL_Window* window);

    bool isFinished() override;

    void update() override;

private:
    int timer = 0;
};
#endif // INTROSCENE_H_INCLUDED
