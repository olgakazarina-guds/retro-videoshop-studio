#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Enum for the dashboard actions (Matches your ofApp)
enum class HomeAction {
    NONE = 0,
    PLAY_VIEW,
    SELECT_MODE,
    UPLOAD_STREAM,
    MANUAL_FILTER
};

class HomeView {
public:
    HomeView();

    // Main draw function matching your ofApp signature
    void draw(cv::Mat& canvas, const cv::Mat& previewFrame);

    // Mouse click hit test
    HomeAction handleMouseClicked(int x, int y);

private:
    int frameCounter;

    // Bounding rectangles for the 4 clickable cards
    cv::Rect btnPlayView;
    cv::Rect btnSelectMode;
    cv::Rect btnUploadStream;
    cv::Rect btnManualFilter;
};
