#include "MediaSourceManager.h"

void MediaSourceManager::loadImage(const std::string& path) {
    ofImage img;
    // 1. Attempt to load the file from the bin/data folder
    if (img.load(path)) {
        ofPixels& pixels = img.getPixels();
        int srcType = (pixels.getImageType() == OF_IMAGE_COLOR_ALPHA) ? CV_8UC4 : CV_8UC3;
        
        // Wrap raw image data into an OpenCV cv::Mat
        cv::Mat temp(static_cast<int>(pixels.getHeight()), static_cast<int>(pixels.getWidth()), srcType, pixels.getData());
        
        // Convert to standard OpenCV BGR format
        if (srcType == CV_8UC4) {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGBA2BGR);
        } else {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
        }
    } 
    else {
        // 2. Fallback: If Test.jpg is not found in bin/data, generate a classic SMPTE broadcast test pattern
        // This ensures the application never starts with a blank gray window!
        ofLogNotice("MediaSourceManager") << "File not found: " << path << ". Generating vintage color-bar test pattern.";
        
        currentFrame = cv::Mat(720, 1280, CV_8UC3);
        
        // 7 Classic SMPTE Color Bars (BGR color format)
        cv::Scalar bars[7] = {
            cv::Scalar(200, 200, 200), // White
            cv::Scalar(0, 220, 220),   // Yellow
            cv::Scalar(220, 220, 0),   // Cyan
            cv::Scalar(0, 220, 0),     // Green
            cv::Scalar(220, 0, 220),   // Magenta
            cv::Scalar(0, 0, 220),     // Red
            cv::Scalar(220, 0, 0)      // Blue
        };
        
        int barW = 1280 / 7;
        for (int i = 0; i < 7; ++i) {
            cv::Rect r(i * barW, 0, (i == 6 ? 1280 - i * barW : barW), 720);
            cv::rectangle(currentFrame, r, bars[i], -1);
        }
        
        // Broadcast test label
        cv::putText(currentFrame, "RETRO VIDEOSHOP - SMPTE TEST PATTERN", cv::Point(320, 360), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(20, 20, 20), 3);
        cv::putText(currentFrame, "RETRO VIDEOSHOP - SMPTE TEST PATTERN", cv::Point(320, 360), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 255, 255), 2);
    }
}

cv::Mat MediaSourceManager::getCurrentFrame() {
    return currentFrame;
}
