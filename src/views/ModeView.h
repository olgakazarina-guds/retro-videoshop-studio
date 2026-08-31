#pragma once

#include <opencv2/opencv.hpp>
#include "../filters/BaseFilter.h"
#include <string>

// ModeView renders a focused single-preset environment with a vintage 35mm film border and intensity scale
class ModeView {
private:
    BaseFilter* currentFilter; // Polymorphic pointer to active filter
    float intensity;           // Scale: 0.0 (subtle) to 1.0 (full effect)
    std::string modeName;

public:
    ModeView();
    ~ModeView() = default;

    void setFilter(BaseFilter* filter, const std::string& name);
    void setIntensity(float val);
    float getIntensity() const { return intensity; }

    void draw(cv::Mat& canvas, const cv::Mat& currentFrame);
};
