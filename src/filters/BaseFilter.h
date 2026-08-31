#pragma once
#include "ofxOpenCv.h"

class BaseFilter
{

public:
	virtual ~BaseFilter () = default;
	virtual cv::Mat process(const cv::Mat& input, float intensity) = 0;
};

