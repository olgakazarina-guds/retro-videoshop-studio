#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Action enum for ofApp state switching
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

    // Renders the viewfinder and dashboard onto the canvas
    void draw(cv::Mat& canvas, const cv::Mat& previewFrame);

    // Mouse click hit test for dashboard cards
    HomeAction handleMouseClicked(int x, int y);

private:
    int frameCounter;

    // Clickable card bounding boxes on 1280x720 canvas
    cv::Rect btnPlayView;
    cv::Rect btnSelectMode;
    cv::Rect btnUploadStream;
    cv::Rect btnManualFilter;
};
