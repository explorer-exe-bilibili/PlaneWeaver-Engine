#pragma once

// 配置项名称定义
// 窗口相关
#define WINDOW_WIDTH "window_width"
#define WINDOW_HEIGHT "window_height"
#define WINDOW_X "window_x"
#define WINDOW_Y "window_y"
#define VERTICAL_SYNC "vertical_sync"
#define WINDOW_TITLE "window_title"

#define DEBUG "debug"
#define SHOW_FPS "show_fps"
#define VOLUME "volume"
#define LANG "lang"
#define INWINDOW "inwindow"

#define UI_REGION_EXIT "ui_region_exit"
#define UI_REGION_EXIT_EDIT "ui_region_exit_edit"
enum class boolconfig {
    unknown = -1, // 未知配置
    inwindow=0,
    debug = 1,
    show_fps = 2,
    vertical_sync = 3
};

extern std::map<boolconfig, bool> bools;

// 从配置名字符串映射到boolconfig枚举
boolconfig GetBoolConfigFromString(const std::string& configName);

struct GLFWwindow;

void fullscreen(GLFWwindow* window);
void defullscreen(GLFWwindow* window);

// 声明bools相关函数
void LoadBoolsFromConfig();
void SyncConfig();
