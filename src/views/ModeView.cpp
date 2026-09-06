#include "ModeView.h"
#include <algorithm>

ModeView::ModeView() : currentFilter(nullptr), intensity(0.75f), modeName("Default Mode") {}

void ModeView::setFilter(BaseFilter* filter, const std::string& name) {
    currentFilter = filter;
    modeName = name;
}

void ModeView::setIntensity(float val) {
    intensity = std::max(0.0f, std::min(1.0f, val));
}

void ModeView::draw(cv::Mat& canvas, const cv::Mat& currentFrame) {
    if (currentFrame.empty()) return;

    canvas = cv::Scalar(18, 18, 20);

    // Apply the polymorphic filter
    cv::Mat filtered;
    if (currentFilter != nullptr) {
        filtered = currentFilter->process(currentFrame, intensity);
    } else {
        filtered = currentFrame.clone();
    }

    // Centered 35mm film view area
    cv::Rect filmRect(140, 60, 1000, 562);
    cv::Mat resized;
    cv::resize(filtered, resized, filmRect.size());
    resized.copyTo(canvas(filmRect));

    // Outer 35mm border
    cv::rectangle(canvas, filmRect, cv::Scalar(80, 80, 85), 3);

    // Header info: Mode Name and Intensity percentage
    cv::putText(canvas, modeName, cv::Point(140, 42), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(240, 240, 240), 2);

    int pct = static_cast<int>(intensity * 100.0f);
    std::string intensityStr = "INTENSITY: " + std::to_string(pct) + "% [+/-]";
    cv::putText(canvas, intensityStr, cv::Point(930, 42), cv::FONT_HERSHEY_PLAIN, 1.3, cv::Scalar(0, 200, 255), 2);

    // Footer instruction
    cv::putText(canvas, "[ESC] Return to Home View", cv::Point(140, 655), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(160, 160, 160), 1);
}
