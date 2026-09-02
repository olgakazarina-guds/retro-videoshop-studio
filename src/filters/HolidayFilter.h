#pragma once

#include "BaseFilter.h"
#include <opencv2/opencv.hpp>

class HolidayFilter : public BaseFilter {
public:
    HolidayFilter() = default;
    virtual ~HolidayFilter() = default;

    // Temporary pass-through until Esther implements color channel shifts
    cv::Mat process(const cv::Mat& input, float intensity) override {
        return input.empty() ? input : input.clone();
    }
};
