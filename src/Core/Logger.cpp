#include <Core/Logger.h>

namespace ShadowRec
{
    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::s_RenderLogger = nullptr;

    void Logger::Init()
    {
        try
        {
            auto coreLogger = spdlog::basic_logger_mt("core", "logs/core.log");
            coreLogger->set_level(spdlog::level::trace);
            coreLogger->flush_on(spdlog::level::warn);
            s_CoreLogger = coreLogger;

            auto renderLogger = spdlog::basic_logger_mt("render", "logs/render.log");
            renderLogger->set_level(spdlog::level::trace);
            renderLogger->flush_on(spdlog::level::warn);
            s_RenderLogger = renderLogger;

            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

            SHADOWREC_INFO("Logger initialized");
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            ::OutputDebugStringA(("Logger init failed: " + std::string(ex.what())).c_str());
        }
    }

    void Logger::Shutdown()
    {
        spdlog::drop("core");
        spdlog::drop("render");
        spdlog::shutdown();
    }
}