#pragma once

#include "BaseFilter.h"
#include <opencv2/opencv.hpp>

class RetroFilter : public BaseFilter {
public:
    RetroFilter() = default;
    virtual ~RetroFilter() = default;

    // Temporary pass-through until Esther implements sepia & grain
    cv::Mat process(const cv::Mat& input, float intensity) override {
        return input.empty() ? input : input.clone();
    }
};
