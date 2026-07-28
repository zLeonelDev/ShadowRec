# ShadowRec

ShadowRec is an open source high-performance screen recorder targeting low-latency gaming capture on Windows.

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](.)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## Building

### Prerequisites

- Windows 10/11
- Visual Studio 2022 Build Tools (v143) or Visual Studio 2022
- CMake 3.16+
- Git

### Quick Start

```bash
git clone https://github.com/zLeonelDev/ShadowRec.git
cd ShadowRec
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The executable will be at `build/bin/Release/ShadowRec.exe`.

### Dependencies (automatically fetched by CMake)

- Dear ImGui v1.90.5
- spdlog v1.12.0
- nlohmann/json v3.11.3

All dependencies are fetched via CMake FetchContent at configure time.

### Project Structure

```
ShadowRec/
├── cmake/              # CMake helper modules
├── src/
│   ├── Core/           # Application, Logger, Config, Events
│   ├── UI/             # Dear ImGui interface
│   └── main.cpp        # Entry point
├── tests/              # Unit tests
└── docs/               # Architecture docs (ADRs)
```

## License

MIT — see [LICENSE](LICENSE) for details.
