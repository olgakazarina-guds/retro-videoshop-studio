#pragma once
#include "BaseFilter.h"
class RetroFilter :
    public BaseFilter {

public:

cv::Mat process(const cv::Mat& input, float intensity) override {
        // Temporary pass-through so it compiles
        return input.clone();
}
};

