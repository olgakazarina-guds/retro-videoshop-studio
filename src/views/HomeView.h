#pragma once

// OpenCV library header for 2D matrix drawing and text rendering
#include <opencv2/opencv.hpp>
#include <string>

// Enum representing the 4 clickable cards on the dashboard
enum class HomeAction {
    NONE = 0,
    PLAY_VIEW,       // Card 1: 2x2 Quad Matrix
    SELECT_MODE,     // Card 2: Presets (Retro, Holiday, Party)
    UPLOAD_STREAM,   // Card 3: Media input switcher
    MANUAL_FILTER    // Card 4: Filter-All manual sandbox
};

class HomeView {
public:
    // Constructor: initializes button bounding boxes
    HomeView();

    // Draws camera chassis, live video, blinking tally, and cards onto canvas
    void draw(cv::Mat& canvas, const cv::Mat& previewFrame);

    // Checks if the user clicked inside any of the 4 cards
    HomeAction handleMouseClicked(int x, int y);

private:
    int frameCounter; // Counter used to time the blinking REC light

    // cv::Rect stores (x, y, width, height) for hit-testing mouse clicks
    cv::Rect btnPlayView;
    cv::Rect btnSelectMode;
    cv::Rect btnUploadStream;
    cv::Rect btnManualFilter;
};
