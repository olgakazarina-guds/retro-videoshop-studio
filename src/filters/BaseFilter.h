#pragma once
#include <ofxOpenCv/openCV.hpp>
class BaseFilter
{

public:
	virtual ~BaseFilter = default;
	virtual cv::Mat process(const cv::Mat& input, float intensity) = 0;
};

