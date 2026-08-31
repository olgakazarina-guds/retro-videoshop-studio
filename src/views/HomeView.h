#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Enum defining the 4 primary interactive choices from the Home Screen dashboard
enum class HomeAction {
    NONE,
    PLAY_VIEW,     // Choice 1: Opens the 2x2 Quad-View matrix
    SELECT_MODE,   // Choice 2: Opens preset fine-tuning view (Retro, Holiday, Party)
    UPLOAD_STREAM, // Choice 3: Opens media source config (Image, Video, Webcam)
    MANUAL_FILTER  // Choice 4: Opens Filter-All Studio sandbox with raw sliders
};

class HomeView {
private:
    // Clickable bounding boxes (Region of Interest / cv::Rect) for the 4 dashboard cards
    cv::Rect btnPlayView;
    cv::Rect btnSelectMode;
    cv::Rect btnUploadStream;
    cv::Rect btnManualFilter;

    // Frame counter used to create the blinking animation for the [REC] camera indicator
    int frameCounter;

public:
    HomeView();
    ~HomeView() = default;

    // Draws the retro camera chassis, live viewfinder monitor, and 4 interactive cards
    void draw(cv::Mat& canvas, const cv::Mat& previewFrame);

    // Hit-testing function: checks (x, y) mouse coordinates against button bounding boxes
    HomeAction handleMouseClicked(int x, int y);
};
