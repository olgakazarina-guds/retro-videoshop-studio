#pragma once

#include <opencv2/opencv.hpp>

// Crop the center of a media frame to the destination aspect ratio before
// resizing it, so circles and other shapes are not stretched on screen.
inline cv::Mat cropToAspectAndResize(const cv::Mat& source, const cv::Size& destinationSize) {
    if (source.empty() || destinationSize.width <= 0 || destinationSize.height <= 0) {
        return cv::Mat();
    }

    const double sourceAspect = static_cast<double>(source.cols) / source.rows;
    const double destinationAspect = static_cast<double>(destinationSize.width) / destinationSize.height;
    cv::Rect crop(0, 0, source.cols, source.rows);

    if (sourceAspect > destinationAspect) {
        const int cropWidth = static_cast<int>(source.rows * destinationAspect);
        crop.x = (source.cols - cropWidth) / 2;
        crop.width = cropWidth;
    } else if (sourceAspect < destinationAspect) {
        const int cropHeight = static_cast<int>(source.cols / destinationAspect);
        crop.y = (source.rows - cropHeight) / 2;
        crop.height = cropHeight;
    }

    cv::Mat resized;
    cv::resize(source(crop), resized, destinationSize);
    return resized;
}
