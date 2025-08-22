#include "core/explorer.h"
#include "core/Config.h"
#include "core/log.h"
#include "core/screen/base.h"

using namespace core;

int Explorer::init()
{
    Log << Level::Info << "Explorer initialization started" << op::endl;
    Config  *config = Config::getInstance();
    // 初始化音频系统 - 注意：SDL_Init应该先于其他SDL组件调用
    if (!initAudio())
    {
        Log << Level::Warn << "音频系统初始化失败，但将继续执行" << op::endl;
    }

    // loadAudio(AudioID::bgm, "path/to/bgm");
    
    Log << Level::Info << "Audio system initialized" << op::endl;

    // loadBitmap(BitmapID::Background, "path/to/background/image");
    loadBitmap(BitmapID::Background, "files/imgs/background.png");

    // 列出所有加载成功的图像
    Log << Level::Info << "Listing all loaded images after initialization:" << op::endl;
    listLoadedBitmaps();
    // 尝试加载默认字体
    try
    {

        // loadFont(FontID::Default, "path/to/default/font", false);

        Log << Level::Info << "Default fonts loaded successfully" << op::endl;
    }
    catch (const std::exception &e)
    {
        Log << Level::Error << "Failed to load font: " << e.what() << op::endl;
    }

    Log << Level::Info << "Preloaded videos successfully" << op::endl;
    Log << Level::Info << "Explorer initialization finished" << op::endl;
    return 0;
}