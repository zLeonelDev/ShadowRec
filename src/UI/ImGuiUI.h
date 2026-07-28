#pragma once

#include <d3d11.h>
#include <imgui.h>

namespace ShadowRec
{
    class ImGuiUI
    {
    public:
        static void BeginFrame();
        static void Render(ID3D11DeviceContext* context);
        static void EndFrame();

        static void OpenSettings();
        static void OpenRecordControl();

        static bool IsSettingsOpen() { return s_ShowSettings; }
        static bool IsRecordControlOpen() { return s_ShowRecordControl; }

    private:
        static bool s_ShowSettings;
        static bool s_ShowRecordControl;

        static void RenderMainMenuBar();
        static void RenderSettingsWindow();
        static void RenderRecordControlWindow();
        static void RenderPerformanceOverlay();
    };
}