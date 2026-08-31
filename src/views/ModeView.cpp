#include "ModeView.h"

ModeView::ModeView() : currentFilter(nullptr), intensity(0.85f), modeName("Preset Mode") {}

void ModeView::setFilter(BaseFilter* filter, const std::string& name) {
    currentFilter = filter;
    modeName = name;
}

void ModeView::setIntensity(float val) {
    // Clamp intensity value between 0.0 and 1.0
    intensity = std::max(0.0f, std::min(1.0f, val));
}

void ModeView::draw(cv::Mat& canvas, const cv::Mat& currentFrame) {
    if (currentFrame.empty()) return;

    // Process frame using dynamic dispatch through the BaseFilter pointer
    cv::Mat processed = (currentFilter != nullptr) 
        ? currentFilter->process(currentFrame, intensity) 
        : currentFrame.clone();

    // Resize processed frame to fit within the 35mm film viewport
    cv::Rect filmFrame(140, 60, 1000, 560);
    cv::Mat resized;
    cv::resize(processed, resized, filmFrame.size());
    canvas = cv::Scalar(12, 12, 14); // Dark vintage frame background
    resized.copyTo(canvas(filmFrame));

    // Draw 35mm sprocket holes along top and bottom edges
    for (int i = 80; i < 1200; i += 70) {
        cv::rectangle(canvas, cv::Rect(i, 15, 30, 30), cv::Scalar(22, 22, 26), -1);
        cv::rectangle(canvas, cv::Rect(i, 640, 30, 30), cv::Scalar(22, 22, 26), -1);
    }

    // Display mode title and current intensity percentage
    cv::putText(canvas, modeName + " (Intensity: " + std::to_string((int)(intensity * 100)) + "%)",
                cv::Point(140, 45), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 200, 255), 2);

    // On-screen control instructions
    cv::putText(canvas, "[+/-] Adjust Intensity   [ESC] Return to Home", 
                cv::Point(140, 650), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 180), 1);
}
