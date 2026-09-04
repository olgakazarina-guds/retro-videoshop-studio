#pragma once

#include "BaseFilter.h"
#include <opencv2/opencv.hpp>

class RetroFilter : public BaseFilter {
public:
    RetroFilter() = default;
    virtual ~RetroFilter() = default;

    cv::Mat process(const cv::Mat& input, float intensity) override;
};