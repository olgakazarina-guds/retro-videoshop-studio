#include "ManualFilter.h"

// Constructor: initialize neutral defaults (contrast = 1.0, brightness = 0, sharpness = 0, no invert)
ManualFilter::ManualFilter() 
    : brightness(0.0f), contrast(1.0f), sharpness(0.0f), invertColors(false) {}

void ManualFilter::setBrightness(float val) { brightness = val; }
void ManualFilter::setContrast(float val)   { contrast = std::max(0.1f, val); }
void ManualFilter::setSharpness(float val)  { sharpness = std::max(0.0f, val); }
void ManualFilter::setInvert(bool val)      { invertColors = val; }

void ManualFilter::reset() {
    brightness = 0.0f;
    contrast = 1.0f;
    sharpness = 0.0f;
    invertColors = false;
}

cv::Mat ManualFilter::process(const cv::Mat& input, float intensity) {
    // Safety check: avoid processing empty frames
    if (input.empty()) return input;

    cv::Mat processed;

    // STEP 1: Linear pixel transform for Contrast (alpha) & Brightness (beta)
    // Formula: Output(x,y) = input(x,y) * contrast + brightness
    input.convertTo(processed, -1, contrast, brightness);

    // STEP 2: Spatial Sharpening using 2D Convolution Kernel (cv::filter2D)
    // Uses a 3x3 Laplacian sharpening matrix
    if (sharpness > 0.01f) {
        cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
             0,           -sharpness,            0,
            -sharpness, 1 + 4 * sharpness, -sharpness,
             0,           -sharpness,            0);
        cv::filter2D(processed, processed, processed.depth(), kernel);
    }

    // STEP 3: Optional Bitwise Color Inversion (255 - pixelValue)
    if (invertColors) {
        cv::bitwise_not(processed, processed);
    }

    // STEP 4: Dynamic weighted blending with source if intensity is less than 1.0
    if (intensity < 0.99f) {
        cv::Mat output;
        cv::addWeighted(input, 1.0f - intensity, processed, intensity, 0.0, output);
        return output;
    }

    return processed;
}
