#include "scenes/Scene.h"

class EditorScene : public Scene {

public:
    explicit EditorScene(SDL_Window* window);

    bool isFinished() override;
};