#include "core/screen/base.h"
#include "core/screen/mainScreen.h"

void RegisterScreens(){
    screen::Screen::RegisterScreen(screen::ScreenID::Main, std::make_shared<screen::MainScreen>());
    screen::Screen::SwitchToScreen(screen::ScreenID::Main);
}