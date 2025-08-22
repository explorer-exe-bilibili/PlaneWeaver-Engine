# make_open_gl_great_again

简体中文 README — 本项目为演示性 C++ 桌面程序，整合 OpenGL 渲染、字体渲染、图片/视频/音频播放与资源管理。

## 概要

该程序展示一个小型多媒体框架：GLFW/GLAD 管理窗口与 OpenGL 上下文；`Explorer` 负责资源加载与缓存（Bitmap、Font、Video、Audio）；FFmpeg + SDL2 负责多媒体解码与播放；FreeType 与 OpenGLFontRenderer 用于文本渲染；并提供多语言本地化支持。

## 主要功能

- 窗口与 OpenGL 上下文管理（GLFW + GLAD）
- 资源管理器（`Explorer`）：位图、字体、视频、音频的加载与缓存
- 视频/音频解码（FFmpeg）与音频播放（SDL2）
- OpenGL 字体渲染后端（OpenGLFontRenderer）
- 配置与日志系统（`core/Config.h`、`core/log.h`）
- 本地化文件在 `files/localization/`

## 仓库结构（简要）

- `src/`：源代码，入口 `src/main.cpp`
- `include/`：公共头文件
- `src/core/`, `include/core/`：核心子系统（Explorer、render、screen、baseItem 等）
- `src/core/render/`：渲染实现（字体渲染、Shader、Texture、VAO/VBO 等）
- `files/`：资源（`fonts/`, `imgs/`, `localization/`）
- `CMakeLists.txt`, `vcpkg.json`：构建脚本与 vcpkg 清单

## 依赖

建议使用 vcpkg 管理依赖（仓库根的 `vcpkg.json` 已列出）。关键依赖示例：glfw3, glad, glm, freetype, ffmpeg, libwebp, sdl2, sdl2-mixer, nlohmann-json, tinyfiledialogs, stb。

注：`CMakeLists.txt` 支持使用 vcpkg（默认）或通过 `-DUSE_SYSTEM_DEPS=ON` 使用系统已安装的库。

## 构建（Windows / PowerShell 示例）

1. 生成构建文件（示例）

```powershell
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_TARGET_TRIPLET=x64-windows \
  -DUSE_SYSTEM_DEPS=OFF \
  -DCMAKE_BUILD_TYPE=Release
```

1. 编译

```powershell
cmake --build build --config Release
```

1. 运行（示例）

```powershell
& .\build\bin\Release\make_open_gl_great_again.exe
```

对于 Linux/macOS，类似流程；或使用 `-DUSE_SYSTEM_DEPS=ON` 并确保系统包管理器提供了所需库。

## 运行时与资源

- 可执行文件默认输出到 `build/bin/<Config>/`（由 CMake 决定）
- 运行时会从 `files/` 加载字体、图片和本地化 JSON
- 动态链接（非 static triplet）时，可能需要将 vcpkg `installed/<triplet>/bin` 下 DLL 复制到可执行目录；CMake 脚本中包含 `copy_dlls` 目标以辅助复制

## 本地化

本地化文件位于 `files/localization/`（JSON 格式）。编辑这些文件可增加或修改界面文本；加载逻辑在 `core/lang.*`（参见 `include/core/baseItem/lang.h`）。

## 调试与常见问题

- OpenGL 版本：程序期望支持 OpenGL 4.0+，驱动/硬件不满足会导致失败
- 字体加载：若自定义字体失败，程序尝试系统字体作为回退
- 多媒体：FFmpeg 或 SDL2 初始化或编解码器缺失会影响音视频，查看日志获取详细信息
- 路径兼容：建议使用 ASCII 项目路径以减少平台兼容问题

## 关键源码文件

- `src/main.cpp`：入口、初始化、主循环、清理
- `include/core/explorer.h` / `src/core/explorer.cpp`：资源管理（Bitmap/Font/Audio/Video）
- `src/core/render/OpenGLFontRenderer.*`：OpenGL 字体渲染实现

## 许可与贡献

本项目采用 MIT 许可证，详见仓库根目录的 [LICENSE](LICENSE) 文件。欢迎提交 issue 或 PR。
