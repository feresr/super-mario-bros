#pragma once

#include "Scene.h"
#include "iostream"

int main(int argc, char** argv);

namespace Engine {

    class Application {
    public:
        explicit Application(const std::string& name = "untitled app",
                             int width = 800, int height = 600,
                             bool fullScreen = false);

        virtual ~Application();

        virtual void handleEvent(SDL_Event& event) = 0;

        static Application& get() { return *instance; }

        bool isRunning = false;

    protected:
        //TODO: use unique ptr?
        Scene* scene = new Scene();

        SDL_Window* window;

        virtual Scene* OnNextScene() { return nullptr; }

    private:
        void run();

        const std::string appName;

        SDL_Event event{};

        static Application* instance;

        friend int::main(int argc, char** argv);
    };

    // To be defined by client
    extern Application* CreateApplication();
}

