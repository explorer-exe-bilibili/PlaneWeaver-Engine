#include "core/screen/mainScreen.h"
#include "core/baseItem/lang.h"

#include "core/log.h"
#include "core/explorer.h"
#include "core/Config.h"

using namespace screen;
using namespace core;

void MainScreen::init() {
    // this function initializes the screen you can register buttons and other UI elements here

    // register background
    background= core::Explorer::getInstance()->getBitmapPtr(BitmapID::Background);

    // register button
    buttons.push_back(std::make_shared<Button>());// buttons comes from base.h
    //then set the button properties

    // SetText can set the button text (in log and UI)
    buttons.back()->SetText("Click Me");
    // SetTextCentered can set the button text to be centered
    buttons.back()->SetTextCentered(true);

    // SetFontID can set the font used to render the button text
    buttons.back()->SetFontID(core::FontID::Default);

    // SetAudioID can set the sound play in click event
    buttons.back()->SetAudioID(core::AudioID::Unknown);

    //you can set bitmap by giving a bitmap ID
    buttons.back()->SetBitmap(core::BitmapID::Unknown);

    // you also have 2 ways to set the region
    //1. provide a region
    buttons.back()->SetRegion({100, 100, 200, 50});
    //2. provide the config name of the region
    buttons.back()->SetRegionStr("button_region");

    // you can set the button background color
    buttons.back()->SetFillColor(core::Color(255, 0, 0, 255));

    // you can switch on or off the button, text, bitmap, background
    buttons.back()->SetEnable(true);
    buttons.back()->SetEnableText(true);
    buttons.back()->SetEnableBitmap(true);
    buttons.back()->SetEnableFill(true);

    // you can set Click function
    buttons.back()->SetClickFunc([]() {
        Log << Level::Info << "Button clicked!" << op::endl;
    });

    // remember to call reloadButtonsRegion() and SetupButtonAlignmentForAllButtons() at the end
    reloadButtonsRegion();
    SetupButtonAlignmentForAllButtons();
}

MainScreen::~MainScreen() {}

bool MainScreen::Click(int x, int y) {
    // call base class's Click method
    // buttons in base class will be automatically checked
    return Screen::Click(x, y);
}

void MainScreen::Draw(){
    // if you don't have other drawing operations, you can simply call the base class's Draw method
    // background, buttons will automatically be drawn
    Screen::Draw();
    // if you need current alpha
    // updateTransition(param); must be called if you don't call Screen::Draw()
    float alpha = getCurrentAlpha();

    //or you can add your own drawing operations here

    // Draw the text
    (*core::Explorer::getInstance()->getFontPtr(FontID::Default))->RenderText("main page",100 ,0,0.5f,Color(255, 255, 255, 255));
    // Draw the bitmap
    (*core::Explorer::getInstance()->getBitmapPtr(BitmapID::Unknown))->Draw({300, 100, 100, 100}, alpha);
    // and so on
}

void MainScreen::enter(int){
    Log << Level::Info << "进入主屏幕" << op::endl;
}