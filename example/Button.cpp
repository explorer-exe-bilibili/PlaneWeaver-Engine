#include "core/screen/mainScreen.h"
#include "core/log.h"


void MainScreen::init() {

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

    // you can move the button smoothly
    buttons.back()->MoveTo({300, 300, 200, 50}, true, 100.0f, [](){
        Log << Level::Info << "Button move completed!" << op::endl;
    });

    // you can let the button fade out
    buttons.back()->FadeOut(1000, [](){
        Log << Level::Info << "Button fade out completed!" << op::endl;
    });
}