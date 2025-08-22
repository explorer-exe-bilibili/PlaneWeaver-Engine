#include "core/Config.h"
#include "core/configItem.h"
#include "core/log.h"
#include "core/baseItem/lang.h"
#include "core/baseItem/Base.h"
#include "core/explorer.h"
#include "core/screen/base.h"

#include <GLFW/glfw3.h>
#ifdef _WIN32
#undef APIENTRY
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
using namespace core::LanguageUtils;

// 实现bools映射
std::map<boolconfig, bool> bools;

// 从配置名字符串映射到boolconfig枚举
boolconfig GetBoolConfigFromString(const std::string& configName) {
    if (configName == INWINDOW) return boolconfig::inwindow;
    if (configName == DEBUG) return boolconfig::debug;
    if (configName == SHOW_FPS) return boolconfig::show_fps;
    if (configName == VERTICAL_SYNC) return boolconfig::vertical_sync;
    return boolconfig::unknown; // 如果没有匹配的配置名，返回未知配置
}

// 从Config加载bool值到bools映射
void LoadBoolsFromConfig() {
    core::Config* config = core::Config::getInstance();
    bools[boolconfig::inwindow] = config->getBool(INWINDOW);
    bools[boolconfig::debug] = config->getBool(DEBUG);
    bools[boolconfig::show_fps] = config->getBool(SHOW_FPS);
}

void fullscreen(GLFWwindow* window){
    if(!window) return;
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    if(!primary) {
        Log << Level::Error << "Failed to get primary monitor for fullscreen" << op::endl;
        return;
    }
    // 更新窗口信息
    core::WindowInfo.width = core::screenInfo.width;
    core::WindowInfo.height = core::screenInfo.height;
    core::WindowInfo.aspectRatio = static_cast<float>(core::screenInfo.width) / core::screenInfo.height;
#ifdef _WIN32
    //用winapi取消全屏独占
    HWND hwnd = glfwGetWin32Window(window);
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) | WS_POPUP);
    ShowWindow(hwnd, SW_MAXIMIZE);
#endif
    // 设置窗口为无边框
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    
    // 设置窗口位置和大小为整个屏幕
    glfwSetWindowPos(window, 0, 0);
    glfwSetWindowSize(window, core::screenInfo.width, core::screenInfo.height);
    
    glViewport(0, 0, core::screenInfo.width, core::screenInfo.height);
    glfwSwapBuffers(window);
    Log << Level::Info << "Switched to fullscreen mode: " << core::screenInfo.width << "x" << core::screenInfo.height << "@" << GLFW_DONT_CARE << "Hz" << op::endl;
}

// 退出无边框全屏模式，切换到普通窗口模式
void defullscreen(GLFWwindow* window){
    if (!window) {
        Log << Level::Error << "窗口指针为空，无法退出全屏" << op::endl;
        return;
    }
    
    // 检查是否处于无边框状态
    if (glfwGetWindowAttrib(window, GLFW_DECORATED)) {
        Log << Level::Warn << "窗口已经是普通窗口模式" << op::endl;
        return;
    }
    
    // 恢复窗口边框
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
    
    // 取消置顶
    glfwSetWindowAttrib(window, GLFW_FLOATING, GLFW_FALSE);
    
    // 从配置中获取窗口参数
    core::Config* config = core::Config::getInstance();
    int windowWidth = config->getInt(WINDOW_WIDTH);
    int windowHeight = config->getInt(WINDOW_HEIGHT);
    int windowX = config->getInt(WINDOW_X);
    int windowY = config->getInt(WINDOW_Y);
    
    // 恢复窗口位置和大小
    glfwSetWindowPos(window, windowX, windowY);
    glfwSetWindowSize(window, windowWidth, windowHeight);
    
    // 更新窗口信息
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    core::WindowInfo.width = width;
    core::WindowInfo.height = height;
    core::WindowInfo.aspectRatio = (float)width / (float)height;
    
    // 更新视口
    glViewport(0, 0, width, height);
    
    Log << Level::Info << "切换到普通窗口模式 (" << windowWidth << "x" << windowHeight << ")" << op::endl;
}

// 将bools映射的值同步到Config
void SyncConfig() {
    core::Config* config = core::Config::getInstance();
    config->set(INWINDOW, bools[boolconfig::inwindow]);
    config->set(DEBUG, bools[boolconfig::debug]);
    config->set(SHOW_FPS, bools[boolconfig::show_fps]);
    config->saveToFile();
    // 设置全屏
    if(!bools[boolconfig::inwindow])fullscreen(core::WindowInfo.window);
    else defullscreen(core::WindowInfo.window);
    core::Explorer::getInstance()->getAudio()->setMusicVolume(core::Config::getInstance()->getUInt(VOLUME));
}

extern std::string GetDefaultLanguage();
void SetConfigItems(){
    core::Config* config = core::Config::getInstance();
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    int screenWidth=1920,screenHeight=1080;
    if(primary){
        const GLFWvidmode* mode =glfwGetVideoMode(primary);
        if(mode){
            core::screenInfo.width=mode->width;
            core::screenInfo.height=mode->height;
            screenHeight=mode->height;
            screenWidth=mode->width;
        }
    }
    else Log<<"Failed to get screen size"<<op::endl;
    config->setScreenSize(screenWidth,screenHeight);
    config->setifno(LANG, GetDefaultLanguage());
    core::LanguageUtils::setLang(core::to_languageID(config->get(LANG)));
    config->setifno(INWINDOW,1);

    config->setifno(WINDOW_WIDTH, screenWidth/2);
    config->setifno(WINDOW_HEIGHT, screenHeight/2);
    config->setifno(WINDOW_X,100);
    config->setifno(WINDOW_Y,100);
    config->setifno(VERTICAL_SYNC, 1);
    config->setifno(WINDOW_TITLE, text("window.title"));
    config->setifno(DEBUG, 0);
    config->setifno(SHOW_FPS,0);
    config->setifno(VOLUME, 100);

    config->setifno(UI_REGION_EXIT, core::Region{0.9,0.03,0.95,-1});
    config->setifno(UI_REGION_EXIT_EDIT, core::Region{0.85,0.4,0.95,0.43});

    // 在设置完配置后，加载bools映射
    LoadBoolsFromConfig();
    
    config->saveToFile();
}