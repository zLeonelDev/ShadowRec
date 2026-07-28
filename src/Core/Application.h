#pragma once

#include <string>
#include <functional>
#include <Windows.h>
#include <d3d11.h>

namespace ShadowRec
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void Stop();

        bool IsRunning() const { return m_Running; }

        ID3D11Device* GetDevice() const { return m_Device; }
        ID3D11DeviceContext* GetContext() const { return m_Context; }
        IDXGISwapChain* GetSwapChain() const { return m_SwapChain; }

        HWND GetWindowHandle() const { return m_hWnd; }

    private:
        bool InitializeWindow(int width, int height, const std::wstring& title);
        bool InitializeDX11(int width, int height);
        bool InitializeImGui();
        void RenderFrame();
        void Cleanup();

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        bool m_Running = false;
        bool m_Initialized = false;

        HINSTANCE m_hInstance = nullptr;
        HWND m_hWnd = nullptr;

        ID3D11Device* m_Device = nullptr;
        ID3D11DeviceContext* m_Context = nullptr;
        IDXGISwapChain* m_SwapChain = nullptr;
        ID3D11RenderTargetView* m_RTV = nullptr;

        int m_WindowWidth = 1280;
        int m_WindowHeight = 720;
        std::wstring m_WindowTitle;
        UINT m_FrameCount = 0;
    };
}