#include "scenes/Scene.h"

class EditorScene : public Scene {

public:
    EditorScene(SDL_Window* window);

private:
    void update() override;

public:
    bool isFinished() override;
};