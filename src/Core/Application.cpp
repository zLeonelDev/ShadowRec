#include <Core/Application.h>
#include <Core/Logger.h>
#include <UI/ImGuiUI.h>
#include <backends/imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <d3dcompiler.h>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace ShadowRec
{
    Application::Application()
        : m_WindowTitle(L"ShadowRec")
    {
    }

    Application::~Application()
    {
        Cleanup();
    }

    void Application::Run()
    {
        if (!InitializeWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle))
        {
            SHADOWREC_ERROR("Failed to initialize window");
            return;
        }

        if (!InitializeDX11(m_WindowWidth, m_WindowHeight))
        {
            SHADOWREC_ERROR("Failed to initialize DirectX 11");
            return;
        }

        if (!InitializeImGui())
        {
            SHADOWREC_ERROR("Failed to initialize ImGui");
            return;
        }

        m_Running = true;
        m_Initialized = true;

        SHADOWREC_INFO("Application running");

        MSG msg = {};
        while (m_Running && msg.message != WM_QUIT)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                RenderFrame();
            }
        }

        SHADOWREC_INFO("Application stopped");
    }

    void Application::Stop()
    {
        m_Running = false;
    }

    bool Application::InitializeWindow(int width, int height, const std::wstring& title)
    {
        m_hInstance = GetModuleHandle(nullptr);

        WNDCLASSEXW wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc;
        wcex.hInstance = m_hInstance;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wcex.lpszClassName = L"ShadowRecWindowClass";

        if (!RegisterClassExW(&wcex))
        {
            SHADOWREC_ERROR("Failed to register window class");
            return false;
        }

        RECT rect = { 0, 0, width, height };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        m_hWnd = CreateWindowExW(
            0,
            L"ShadowRecWindowClass",
            title.c_str(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr, nullptr, m_hInstance, this);

        if (!m_hWnd)
        {
            SHADOWREC_ERROR("Failed to create window");
            return false;
        }

        ShowWindow(m_hWnd, SW_SHOW);
        UpdateWindow(m_hWnd);

        return true;
    }

    bool Application::InitializeDX11(int width, int height)
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = m_hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &sd,
            &m_SwapChain,
            &m_Device,
            &featureLevel,
            &m_Context);

        if (FAILED(hr))
        {
            SHADOWREC_ERROR("D3D11CreateDeviceAndSwapChain failed with HRESULT 0x{:08X}", hr);
            return false;
        }

        ID3D11Texture2D* backBuffer = nullptr;
        hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
        if (FAILED(hr))
        {
            SHADOWREC_ERROR("Failed to get swap chain back buffer");
            return false;
        }

        hr = m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RTV);
        backBuffer->Release();

        if (FAILED(hr))
        {
            SHADOWREC_ERROR("Failed to create render target view");
            return false;
        }

        m_Context->OMSetRenderTargets(1, &m_RTV, nullptr);

        D3D11_VIEWPORT vp = {};
        vp.Width = static_cast<float>(width);
        vp.Height = static_cast<float>(height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_Context->RSSetViewports(1, &vp);

        SHADOWREC_INFO("DirectX 11 initialized (feature level {})", static_cast<int>(featureLevel));
        return true;
    }

    bool Application::InitializeImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(m_hWnd);
        ImGui_ImplDX11_Init(m_Device, m_Context);

        SHADOWREC_INFO("ImGui initialized");
        return true;
    }

    void Application::RenderFrame()
    {
        const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        m_Context->ClearRenderTargetView(m_RTV, clearColor);

        ImGuiUI::BeginFrame();
        ImGuiUI::Render(m_Context);
        ImGuiUI::EndFrame();

        m_SwapChain->Present(1, 0);
        m_FrameCount++;
    }

    void Application::Cleanup()
    {
        if (m_Initialized)
        {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }

        if (m_RTV)
        {
            m_RTV->Release();
            m_RTV = nullptr;
        }

        if (m_SwapChain)
        {
            m_SwapChain->Release();
            m_SwapChain = nullptr;
        }

        if (m_Context)
        {
            m_Context->Release();
            m_Context = nullptr;
        }

        if (m_Device)
        {
            m_Device->Release();
            m_Device = nullptr;
        }

        if (m_hWnd)
        {
            DestroyWindow(m_hWnd);
            m_hWnd = nullptr;
        }

        UnregisterClassW(L"ShadowRecWindowClass", m_hInstance);

        SHADOWREC_INFO("Application cleaned up");
    }

    LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Application* app = nullptr;

        if (message == WM_NCCREATE)
        {
            const CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            app = reinterpret_cast<Application*>(pCreate->lpCreateParams);
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
        }
        else
        {
            app = reinterpret_cast<Application*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        }

        if (app)
        {
            if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
                return TRUE;
        }

        switch (message)
        {
        case WM_SIZE:
            if (app && app->m_Device && wParam != SIZE_MINIMIZED)
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);

                if (app->m_RTV)
                {
                    app->m_RTV->Release();
                    app->m_RTV = nullptr;
                }

                app->m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

                ID3D11Texture2D* backBuffer = nullptr;
                app->m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
                app->m_Device->CreateRenderTargetView(backBuffer, nullptr, &app->m_RTV);
                backBuffer->Release();

                app->m_Context->OMSetRenderTargets(1, &app->m_RTV, nullptr);

                D3D11_VIEWPORT vp = {};
                vp.Width = static_cast<float>(width);
                vp.Height = static_cast<float>(height);
                vp.MinDepth = 0.0f;
                vp.MaxDepth = 1.0f;
                app->m_Context->RSSetViewports(1, &vp);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CLOSE:
            if (app)
                app->Stop();
            return 0;
        }

        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
}