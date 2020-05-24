#include "scenes/Scene.h"

class GameScene : public Scene {

public:
    GameScene(SDL_Window* window);

private:
    void update() override;

};