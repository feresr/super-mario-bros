#include "../../engine/include/Application.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "IntroScene.h"
#include "Constants.h"

class SandboxApplication : public Engine::Application {

public:
    SandboxApplication() : Engine::Application("Super Mario Bros",
                                               SNES_RESOLUTION_WIDTH * ZOOM,
                                               SNES_RESOLUTION_HEIGHT * ZOOM,
                                               false) {
        scene = new IntroScene(window);
    }

    Scene* OnNextScene() override {
        return new GameScene(window);
    }

    void handleEvent(SDL_Event& event) override {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            return;
        }
        scene->handleEvents(event);
        switch (event.type) {
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_E:
                        if (dynamic_cast<GameScene*>(scene)) {
                            delete scene;
                            scene = new EditorScene(window);
                        } else {
                            delete scene;
                            scene = new GameScene(window);
                        }
                        break;
                    default:
                        break;
                }
                break;
        }
    }
};

Engine::Application* Engine::CreateApplication() {
    return new SandboxApplication();
}