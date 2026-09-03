#pragma once
#include "BaseFilter.h"
class RetroFilter :
    public BaseFilter {

public:
	RetroFilter() = default;
	virtual ~RetroFilter() = default;

cv::Mat process(const cv::Mat& input, float intensity) override;

};

