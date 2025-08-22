#pragma once

#include "../render/Texture.h"
#include "Bitmap.h"
#include "Font.h"
#include <functional>
#include <mutex>
#include <atomic>
#include <memory>
#include "../explorer.h"
#include "../Config.h"
#include <vector>

// Enum for edit modes
enum class EditMode {
    None,           // Not in edit mode
    Move,           // Move mode
    ResizeTopLeft,  // Resize top-left corner
    ResizeTopRight, // Resize top-right corner
    ResizeBottomLeft,  // Resize bottom-left corner
    ResizeBottomRight, // Resize bottom-right corner
    ResizeLeft,     // Resize left side
    ResizeRight,    // Resize right side
    ResizeTop,      // Resize top side
    ResizeBottom    // Resize bottom side
};

// Structure for edit handles
struct EditHandle {
    core::Region region;
    EditMode mode;
    bool visible;
    EditHandle() : region(), mode(EditMode::None), visible(false) {}
    EditHandle(const core::Region& r, EditMode m) : region(r), mode(m), visible(true) {}
};

namespace core {
class Button {
public:
    Button(const std::string& text="", FontID fontid=FontID::Default, const Region& region=Region(), Bitmap** bitmapPtr=nullptr);
    Button(const Button& button);
    Button& operator=(const Button& button);
    ~Button();

    void Draw(unsigned char alpha=255); // Draw the button
    bool OnClick(Point point); // Handle click event
    void MoveTo(const Region& region, const bool enableFluent=false, const float speed=50.0f, std::function<void()> onComplete=nullptr); // Move to a region
    void FadeOut(float duration, unsigned char startAlpha=255, unsigned char endAlpha=0, std::function<void()> onComplete=nullptr); // Fade out
    void SetClickFunc(std::function<void()> func) {this->ClickFunc = func;} // Set click event function

    void SetText(const std::string& text) {this->text = text;} // Set button text (displayed in UI and log)
    void SetBitmap(const std::string& bitmapID) { // Set bitmap using a string identifier
        if(core::Explorer::getInstance()->isBitmapLoaded(bitmapID)) {
            this->bitmapPtr = core::Explorer::getInstance()->getBitmapPtr(bitmapID);
            UpdateImageAspectRatio();
        }
    }
    void SetBitmap(BitmapID id) { // Set bitmap using an ID
        this->bitmapid = id;
        if(core::Explorer::getInstance()->isBitmapLoaded(id)) {
            this->bitmapPtr = core::Explorer::getInstance()->getBitmapPtr(id);
            UpdateImageAspectRatio();
        }
    }
    void SetRegion(const Region& region) {MoveTo(region); UpdateEditHandles();} // Set region (instant move)
    void SetRegionStr(const std::string& name){this->regionConfig=name;resetRegion();} // Set region configuration name for auto-save and auto-apply
    void SetFontID(FontID id); // Set font ID
    void SetAudioID(AudioID id) {this->audioid = id;} // Set sound for button click (must be loaded with loadSound())
    void SetTextCentered(bool isCentered){this->isCentered = isCentered;} // Set text alignment to center
    void SetTextColor(const Color& color) {this->color = color;} // Set text color
    void SetFillColor(const Color& color) {this->fillColor = color;} // Set background fill color
    void SetFontScale(float scale) { this->fontScale = scale; } // Set font scale

    void SetEnableText(bool enable) {this->enableText = enable;} // Enable/disable text display
    void SetEnable(bool enable) {this->enable = enable;} // Enable/disable button
    bool IsEnable() const {return enable;} // Get enable status
    void SetEnableBitmap(bool enable) {this->enableBitmap = enable;} // Enable/disable bitmap display
    void SetEnableFill(bool enable) {this->enableFill = enable;} // Enable/disable background fill
    void SetAspectRatioSnap(bool enable) {enableAspectRatioSnap = enable;} // Enable/disable aspect ratio snapping
    void SetCenterSnap(bool enable) {enableCenterSnap = enable;} // Enable/disable center snapping
    void SetCustomSnap(bool enable) {enableCustomSnap = enable;} // Enable/disable custom snapping
    void SetButtonAlignSnap(bool enable) {enableButtonAlignSnap = enable;} // Enable/disable button alignment snapping

    void resetRegion(); // Read region configuration and apply

    // Methods related to edit mode // Do not modify lightly
    void SetEditMode(bool enable); // Enable/disable edit mode
    bool IsEditModeEnabled() const {return editModeEnabled;}
    void SetEditHandleSize(float size) {editHandleSize = size;UpdateEditHandles();}
    void SetEditHandleColor(const Color& color) {editHandleColor = color;}
    void SetEditBorderColor(const Color& color) {editBorderColor = color;}
    void SetEditBorderWidth(float width) {editBorderWidth = width;}
    void SetOnEditComplete(std::function<void(const Region&)> callback);
    bool OnEditMouseDown(Point point);
    bool OnEditMouseMove(Point point);
    bool OnEditMouseUp(Point point);
    void DrawEditOverlay();
    void SaveRegionToConfig() const {
        if(regionConfig.empty()) return;
        Config::getInstance()->set(this->regionConfig, this->region);
    }
    EditMode GetEditModeAt(Point point) const;

    // Aspect ratio snapping functionality
    bool IsAspectRatioSnapEnabled() const {return enableAspectRatioSnap;}
    void SetAspectRatioSnapThreshold(float threshold) {aspectRatioSnapThreshold = threshold;}
    float GetImageAspectRatio() const {return originalImageAspectRatio;}

    // Center snapping functionality
    bool IsCenterSnapEnabled() const {return enableCenterSnap;}
    void SetCenterSnapThreshold(float threshold) {centerSnapThreshold = threshold;}

    // Custom snapping points functionality
    void AddCustomSnapX(float x);                  // Add custom X-axis snapping point
    void AddCustomSnapY(float y);                  // Add custom Y-axis snapping point
    void RemoveCustomSnapX(float x);               // Remove specific X-axis snapping point
    void RemoveCustomSnapY(float y);               // Remove specific Y-axis snapping point
    void ClearCustomSnapX();                       // Clear all X-axis snapping points
    void ClearCustomSnapY();                       // Clear all Y-axis snapping points
    bool IsCustomSnapEnabled() const {return enableCustomSnap;}
    void SetCustomSnapThreshold(float threshold) {customSnapThreshold = threshold;}
    const std::vector<float>& GetCustomSnapX() const {return customSnapX;}
    const std::vector<float>& GetCustomSnapY() const {return customSnapY;}

    // Button alignment snapping functionality
    bool IsButtonAlignSnapEnabled() const {return enableButtonAlignSnap;}
    void SetButtonAlignSnapThreshold(float threshold) {buttonAlignSnapThreshold = threshold;}
    void SetOtherButtonsForAlignment(const std::vector<std::shared_ptr<Button>>& otherButtons) {otherButtonsPtr = &otherButtons;}
    const std::vector<std::shared_ptr<Button>>* GetOtherButtonsPtr() const {return otherButtonsPtr;}
    
    Region GetRegion() const { return region; }
    std::string GetText() const { return text; }
protected:
    Region region;
    Color color=Color(255, 255, 255, 255);
    Color fillColor=Color(0, 0, 0, 255);
    bool enableText=true;
    bool enable=true;
    bool enableBitmap=true;
    bool enableFill=false;
    bool isCentered=true; // Whether to center the text
    std::string text="";
    std::string regionConfig="";
    Bitmap** bitmapPtr = nullptr; // Pointer for automatic updates
    std::function<void()> ClickFunc;
    BitmapID bitmapid=BitmapID::Unknown;
    FontID fontid=FontID::Default;
    AudioID audioid=AudioID::Unknown;
    float fontSize=0;
    float fontScale=1.0f;
    Font** fontPtr = nullptr; // Pointer for automatic font updates
    // Thread safety members
    std::timed_mutex animMutex;                   // Thread synchronization mutex (supports timeout)
    std::atomic<bool> animationRunning{false}; // Flag indicating if animation is running
    std::atomic<bool> stopRequested{false}; // Stop request flag
    
    // Fade-out animation members
    std::atomic<bool> fadeAnimationRunning{false}; // Flag indicating if fade-out animation is running
    std::atomic<bool> fadeStopRequested{false}; // Stop request flag for fade-out animation
    std::atomic<unsigned char> currentFadeAlpha{255}; // Current fade-out alpha value
    mutable std::timed_mutex fadeMutex;           // Thread synchronization mutex for fade-out animation (supports timeout)

    // Edit mode members
    bool editModeEnabled = false;
    EditMode currentEditMode = EditMode::None;
    bool isDragging = false;
    Point dragStartPoint;
    Region originalRegion;
    std::vector<EditHandle> editHandles;
    float editHandleSize = 8.0f;
    Color editHandleColor = Color(0, 120, 215, 255);
    Color editBorderColor = Color(0, 120, 215, 255);
    float editBorderWidth = 2.0f;
    std::function<void(const Region&)> onEditComplete;
    float minWidth = 10.0f;
    float minHeight = 10.0f;
    
    // Aspect ratio snapping members
    static bool enableAspectRatioSnap; // Default enabled snapping functionality
    static float aspectRatioSnapThreshold; // Snapping threshold, triggered when the difference between current and original aspect ratio is less than this value
    float originalImageAspectRatio = 1.0f; // Original image aspect ratio
    bool hasValidImageAspectRatio = false; // Whether a valid image aspect ratio exists

    // Center snapping members
    static bool enableCenterSnap; // Whether to enable center snapping
    static float centerSnapThreshold; // Center snapping threshold (pixels)
    bool isSnappedToCenter = false; // Whether currently snapped to center

    // Custom snapping points members
    static bool enableCustomSnap; // Whether to enable custom snapping
    static float customSnapThreshold; // Custom snapping threshold (pixels)
    std::vector<float> customSnapX; // List of custom X-axis snapping points
    std::vector<float> customSnapY; // List of custom Y-axis snapping points
    bool isSnappedToCustom = false; // Whether currently snapped to custom points

    // Button alignment snapping members
    static bool enableButtonAlignSnap; // Whether to enable button alignment snapping
    static float buttonAlignSnapThreshold; // Button alignment snapping threshold
    const std::vector<std::shared_ptr<Button>>* otherButtonsPtr = nullptr; // Reference to other buttons
    bool isSnappedToButton = false; // Whether currently snapped to other buttons

    // Edit mode helper methods
    void UpdateEditHandles();
    void UpdateRegionFromEdit(Point currentPoint);
    bool IsPointInHandle(Point point, const EditHandle& handle) const;
    void ClampRegion();
    
private:
    // Aspect ratio snapping helper methods
    void UpdateImageAspectRatio();
    bool ShouldSnapToAspectRatio(float currentAspectRatio) const;
    void ApplyAspectRatioSnap(Region& targetRegion) const;

    // Center snapping helper methods
    bool ShouldSnapToCenter(const Region& targetRegion) const;
    void ApplyCenterSnap(Region& targetRegion) const;

    // Custom snapping points helper methods
    bool ShouldSnapToCustomPoints(const Region& targetRegion, float& snapX, float& snapY) const;
    void ApplyCustomSnap(Region& targetRegion, float snapX, float snapY) const;

    // Button alignment snapping helper methods
    bool ShouldSnapToOtherButtons(const Region& targetRegion, float& snapX, float& snapY) const;
    void ApplyButtonAlignSnap(Region& targetRegion, float snapX, float snapY) const;
};
}