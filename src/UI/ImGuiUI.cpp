#include <UI/ImGuiUI.h>
#include <Core/Application.h>
#include <Core/Logger.h>
#include <Core/Config.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

namespace ShadowRec
{
    bool ImGuiUI::s_ShowSettings = false;
    bool ImGuiUI::s_ShowRecordControl = false;

    void ImGuiUI::BeginFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiUI::Render(ID3D11DeviceContext* context)
    {
        (void)context;

        RenderMainMenuBar();
        RenderSettingsWindow();
        RenderRecordControlWindow();
        RenderPerformanceOverlay();
    }

    void ImGuiUI::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiUI::OpenSettings()
    {
        s_ShowSettings = true;
    }

    void ImGuiUI::OpenRecordControl()
    {
        s_ShowRecordControl = true;
    }

    void ImGuiUI::RenderMainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Settings"))
                    s_ShowSettings = true;
                if (ImGui::MenuItem("Record"))
                    s_ShowRecordControl = true;
                if (ImGui::MenuItem("Exit"))
                    PostQuitMessage(0);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void ImGuiUI::RenderSettingsWindow()
    {
        if (!s_ShowSettings)
            return;

        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Settings", &s_ShowSettings);

        auto& config = Config::Get();

        ImGui::Text("Video Capture");
        ImGui::Separator();

        auto captureMethod = config.Get<std::string>("capture_method", "dxgi");
        if (ImGui::BeginCombo("Capture Method", captureMethod.c_str()))
        {
            if (ImGui::Selectable("DXGI Desktop Duplication", captureMethod == "dxgi"))
            {
                config.Set("capture_method", "dxgi");
            }
            if (ImGui::Selectable("Windows Graphics Capture", captureMethod == "wgc"))
            {
                config.Set("capture_method", "wgc");
            }
            ImGui::EndCombo();
        }

        auto fpsLimit = config.Get<int>("fps_limit", 60);
        ImGui::SliderInt("FPS Limit", &fpsLimit, 1, 240);
        config.Set("fps_limit", fpsLimit);

        auto resWidth = config.Get<int>("resolution_width", 1920);
        auto resHeight = config.Get<int>("resolution_height", 1080);
        ImGui::InputInt("Width", &resWidth);
        ImGui::InputInt("Height", &resHeight);
        config.Set("resolution_width", resWidth);
        config.Set("resolution_height", resHeight);

        auto codec = config.Get<std::string>("codec", "h264");
        if (ImGui::BeginCombo("Codec", codec.c_str()))
        {
            const char* codecs[] = { "h264", "h265", "av1", "none" };
            for (const char* c : codecs)
            {
                bool selected = (codec == c);
                if (ImGui::Selectable(c, selected))
                {
                    config.Set("codec", c);
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();
        ImGui::Text("Audio");
        ImGui::Separator();

        char audioDeviceBuf[256] = {};
        auto audioDevice = config.Get<std::string>("audio_device", "default");
        strncpy_s(audioDeviceBuf, sizeof(audioDeviceBuf), audioDevice.c_str(), _TRUNCATE);
        if (ImGui::InputText("Audio Device", audioDeviceBuf, sizeof(audioDeviceBuf)))
        {
            config.Set("audio_device", std::string(audioDeviceBuf));
        }

        auto audioLatency = config.Get<std::string>("audio_latency", "medium");
        if (ImGui::BeginCombo("Audio Latency", audioLatency.c_str()))
        {
            const char* latencies[] = { "low", "medium", "high" };
            for (const char* l : latencies)
            {
                bool selected = (audioLatency == l);
                if (ImGui::Selectable(l, selected))
                {
                    config.Set("audio_latency", l);
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();
        ImGui::Text("Output");
        ImGui::Separator();

        char outputPathBuf[512] = {};
        auto outputPath = config.Get<std::string>("output_path", "./recordings");
        strncpy(outputPathBuf, outputPath.c_str(), sizeof(outputPathBuf) - 1);
        if (ImGui::InputText("Output Directory", outputPathBuf, sizeof(outputPathBuf)))
        {
            config.Set("output_path", std::string(outputPathBuf));
        }

        ImGui::End();
    }

    void ImGuiUI::RenderRecordControlWindow()
    {
        if (!s_ShowRecordControl)
            return;

        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
        ImGui::Begin("Recording", &s_ShowRecordControl);

        ImGui::Text("Status: Idle");

        if (ImGui::Button("Start Recording", ImVec2(150, 40)))
        {
            SHADOWREC_INFO("Recording started");
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop Recording", ImVec2(150, 40)))
        {
            SHADOWREC_INFO("Recording stopped");
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Output file: recording.mp4");
        ImGui::Text("Duration: 00:00:00");
        ImGui::Text("File size: 0 MB");

        ImGui::End();
    }

    void ImGuiUI::RenderPerformanceOverlay()
    {
        if (!s_ShowRecordControl)
            return;

        ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);

        ImGui::Begin("Performance", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("FPS: 60");
        ImGui::Text("Frame latency: 16.67 ms");
        ImGui::Text("Capture latency: 0.5 ms");
        ImGui::Text("Encode latency: 2.1 ms");

        ImGui::End();
    }
}