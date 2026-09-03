#pragma once
#include "BaseFilter.h"
class HolidayFilter :
    public BaseFilter {

public:
	HolidayFilter() = default;
	virtual ~HolidayFilter() = default;

	cv::Mat process(const cv::Mat& input, float intensity) override;
	};

