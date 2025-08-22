# make_open_gl_great_again

For the Chinese version, see: [README.zh-CN.md](README.zh-CN.md)

## Overview

make_open_gl_great_again is a small demo C++ desktop application that showcases OpenGL rendering, font rendering, image/video/audio playback, and a simple resource manager.

The project uses GLFW/GLAD for window and OpenGL context management, an Explorer resource manager for bitmaps/fonts/videos/audios, FFmpeg + SDL2 for media decoding and audio playback, and FreeType + a custom OpenGL font renderer for text.

## Key features

- Window and OpenGL context management (GLFW + GLAD)
- Resource manager (Explorer) for Bitmap / Font / Video / Audio
- Video/audio decoding with FFmpeg and playback via SDL2
- OpenGL font rendering backend (OpenGLFontRenderer)

## Repository layout (short)

- `src/` - source code (entry: `src/main.cpp`)
- `include/` - public headers
- `src/core/`, `include/core/` - core subsystems
- `files/` - resources (`fonts/`, `imgs/`, `localization/`)
- `CMakeLists.txt`, `vcpkg.json` - build scripts and vcpkg manifest

## Dependencies

Recommended: use vcpkg (see `vcpkg.json`). Typical packages include: glfw3, glad, glm, freetype, ffmpeg, libwebp, sdl2, sdl2-mixer, nlohmann-json, tinyfiledialogs, stb.

Set `-DUSE_SYSTEM_DEPS=ON` if you prefer system-provided libraries instead of the vcpkg manifest.

## Build (Windows / PowerShell example)

1. Configure (example)

    ```powershell
    cmake -S . -B build \
      -DCMAKE_TOOLCHAIN_FILE=C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake \
      -DVCPKG_TARGET_TRIPLET=x64-windows \
      -DUSE_SYSTEM_DEPS=OFF \
      -DCMAKE_BUILD_TYPE=Release
    ```

2. Build

    ```powershell
    cmake --build build --config Release
    ```

3. Run (example)

    ```powershell
    & .\build\bin\Release\make_open_gl_great_again.exe
    ```

For Linux/macOS, use a similar flow or set `-DUSE_SYSTEM_DEPS=ON` and ensure required libraries are installed on the system.

## Runtime & resources

- Executable is placed in `build/bin/<Config>/` (default)
- Runtime loads fonts, images and localization JSON from `files/`
- When using dynamic linking, you may need to copy vcpkg's `installed/<triplet>/bin` DLLs to the executable folder; CMake includes a `copy_dlls` helper target

## Localization

Localization JSON files are in `files/localization/`. Edit these files to add or update UI strings. Loading logic is implemented under `src/core/baseItem/lang.*` and related headers.

## Troubleshooting

- OpenGL: the app expects OpenGL 4.0+. Older GPUs or drivers may fail to initialize the context.
- Fonts: the app has fallback logic to system fonts if bundled fonts are unavailable.
- Media: missing FFmpeg codecs or SDL2 runtime issues will affect audio/video playback; check console logs for details.

## Key source files

- `src/main.cpp` — program entry, initialization, main loop, cleanup
- `include/core/explorer.h` and `src/core/explorer.cpp` — resource manager (Bitmap/Font/Audio/Video)
- `src/core/render/OpenGLFontRenderer.*` — OpenGL font renderer implementation used at runtime