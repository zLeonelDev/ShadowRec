#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <memory>

namespace ShadowRec
{
    class Logger
    {
    public:
        static void Init();
        static void Shutdown();

        static std::shared_ptr<spdlog::logger>& Get()
        {
            return s_CoreLogger;
        }

        static std::shared_ptr<spdlog::logger>& GetRender()
        {
            return s_RenderLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_RenderLogger;
    };
}

#define SHADOWREC_TRACE(...)    ::ShadowRec::Logger::Get()->trace(__VA_ARGS__)
#define SHADOWREC_INFO(...)     ::ShadowRec::Logger::Get()->info(__VA_ARGS__)
#define SHADOWREC_WARN(...)     ::ShadowRec::Logger::Get()->warn(__VA_ARGS__)
#define SHADOWREC_ERROR(...)    ::ShadowRec::Logger::Get()->error(__VA_ARGS__)
#define SHADOWREC_CRITICAL(...) ::ShadowRec::Logger::Get()->critical(__VA_ARGS__)

#define SHADOWREC_RENDER_TRACE(...) ::ShadowRec::Logger::GetRender()->trace(__VA_ARGS__)
#define SHADOWREC_RENDER_INFO(...)  ::ShadowRec::Logger::GetRender()->info(__VA_ARGS__)
#define SHADOWREC_RENDER_WARN(...)  ::ShadowRec::Logger::GetRender()->warn(__VA_ARGS__)
#define SHADOWREC_RENDER_ERROR(...) ::ShadowRec::Logger::GetRender()->error(__VA_ARGS__)
#define SHADOWREC_RENDER_CRITICAL(...)::ShadowRec::Logger::GetRender()->critical(__VA_ARGS__)