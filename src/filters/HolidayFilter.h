#pragma once
#include "BaseFilter.h"
class HolidayFilter :
    public BaseFilter {

public:
	cv::Mat process(const cv::Mat& input, float intensity) override {
        // Temporary pass-through so it compiles
        return input.clone();
	}
	};

