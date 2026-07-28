cmake_minimum_required(VERSION 3.16)
project(ShadowRec VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

include(cmake/Dependencies.cmake)
include(cmake/CompilerWarnings.cmake)

set(SHADOWREC_SOURCES
    src/main.cpp
    src/Core/Logger.cpp
    src/Core/Config.cpp
    src/Core/Application.cpp
    src/UI/ImGuiUI.cpp
)

add_executable(ShadowRec WIN32 ${SHADOWREC_SOURCES})

target_include_directories(ShadowRec PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Core
    ${CMAKE_SOURCE_DIR}/src/UI
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

target_link_libraries(ShadowRec PRIVATE
    spdlog::spdlog_header_only
    nlohmann_json::nlohmann_json
    DearImGui::ImGui
    DearImGui::imgui_impl_dx11
    DearImGui::imgui_impl_win32
    d3d11
    dxgi
)

target_compile_definitions(ShadowRec PRIVATE
    SHADOWREC_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    SHADOWREC_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    SHADOWREC_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    $<$<CONFIG:Debug>:SHADOWREC_DEBUG>
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
)

target_compile_options(ShadowRec PRIVATE /W4 /sdl /permissive-)

set_target_properties(ShadowRec PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)