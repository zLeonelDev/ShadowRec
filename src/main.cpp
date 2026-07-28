#include <Core/Logger.h>
#include <Core/Application.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    ShadowRec::Logger::Init();
    SHADOWREC_INFO("ShadowRec v{}.{}.{} starting",
                   SHADOWREC_VERSION_MAJOR,
                   SHADOWREC_VERSION_MINOR,
                   SHADOWREC_VERSION_PATCH);

    try
    {
        ShadowRec::Application app;
        app.Run();
    }
    catch (const std::exception& ex)
    {
        SHADOWREC_ERROR("Fatal exception: {}", ex.what());
        return EXIT_FAILURE;
    }

    SHADOWREC_INFO("ShadowRec terminated cleanly");
    return EXIT_SUCCESS;
}