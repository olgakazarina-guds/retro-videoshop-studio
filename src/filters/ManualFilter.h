#pragma once

#include "BaseFilter.h"
#include <opencv2/opencv.hpp>

// ManualFilter inherits from BaseFilter ("Is-A" Polymorphic relationship).
// It acts as a manual sandbox, allowing raw slider tweaking of brightness, contrast, sharpness, and color inversion.
class ManualFilter : public BaseFilter
{
private:
    float brightness;  // beta offset: range -100 to 100
    float contrast;    // alpha scale factor: range 0.1 to 3.0
    float sharpness;   // convolution kernel weight: range 0.0 to 5.0
    bool invertColors; // flag to enable full-frame bitwise inversion

public:
    // Constructor: initializes parameters to neutral default values
    ManualFilter();
    virtual ~ManualFilter() = default;

    // BaseFilter interface implementation
    cv::Mat process(const cv::Mat& input, float intensity = 1.0f) override;

    // Setter methods for UI sliders and interactive key controls
    void setBrightness(float val);
    void setContrast(float val);
    void setSharpness(float val);
    void setInvert(bool val);

    // Helper method to reset all adjustments back to default
    void reset();
};
