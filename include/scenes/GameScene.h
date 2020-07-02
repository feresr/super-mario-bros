#include "scenes/Scene.h"

class GameScene : public Scene {

public:
    explicit GameScene(SDL_Window* window);

private:
    bool gameOver = false;
    void update() override;

public:
    bool isFinished() override;

};