#include "scenes/Scene.h"

class IntroScene : public Scene {

public:
    IntroScene(SDL_Window* window);

private:
    void update() override;

};