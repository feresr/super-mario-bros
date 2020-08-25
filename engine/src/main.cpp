#include "Application.h"

#include "Log.h"

int main(int argc, char** argv) {
    Engine::Log::init();
    ENGINE_CORE_INFO("Launching app...");
    Engine::Application* application = Engine::CreateApplication();
    application->run();
    delete application;
    return 0;
}
