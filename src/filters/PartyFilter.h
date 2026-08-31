#pragma once

#include "BaseFilter.h"
#include <opencv2/opencv.hpp>

// PartyFilter inherits from BaseFilter ("Is-A" Polymorphism relationship).
// It overrides the pure virtual method 'process()' to apply neon and color-inversion effects.
class PartyFilter : public BaseFilter
{
public:
    PartyFilter() = default;
    virtual ~PartyFilter() = default;

    // Overridden processing function that transforms the input frame based on intensity (0.0 to 1.0)
    cv::Mat process(const cv::Mat& input, float intensity) override;
};
