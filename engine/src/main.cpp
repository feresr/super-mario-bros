#include "Application.h"

int main(int argc, char** argv) {
    Engine::Application* application = Engine::CreateApplication();
    application->run();
    delete application;
    return 0;
}
