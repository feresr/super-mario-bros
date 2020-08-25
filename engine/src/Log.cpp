#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace Engine {

    std::shared_ptr<spdlog::logger> Log::coreLogger;
    std::shared_ptr<spdlog::logger> Log::clientLogger;

    void Log::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        coreLogger = spdlog::stdout_color_mt("ENGINE");
        coreLogger->set_level(spdlog::level::level_enum::trace);
        clientLogger = spdlog::stdout_color_mt("APP");
        clientLogger->set_level(spdlog::level::level_enum::trace);
    }

    Log::Log() {}

    Log::~Log() {}

}