#pragma once

#include <opencv2/opencv.hpp>
#include "../filters/ManualFilter.h"

// FilterStudioView renders the manual keyboard-controlled filter workspace.
class FilterStudioView {
private:
    ManualFilter manualFilter; // Owns the ManualFilter instance

public:
    FilterStudioView() = default;
    ~FilterStudioView() = default;

    ManualFilter& getFilter() { return manualFilter; }
    void draw(cv::Mat& canvas, const cv::Mat& currentFrame);
};
