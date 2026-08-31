#include "FilterStudioView.h"

void FilterStudioView::draw(cv::Mat& canvas, const cv::Mat& currentFrame) {
    if (currentFrame.empty()) return;

    // Process current frame through the manual sandbox filter
    cv::Mat processed = manualFilter.process(currentFrame, 1.0f);
    cv::Rect previewRect(60, 60, 800, 560);
    cv::Mat resized;
    cv::resize(processed, resized, previewRect.size());

    canvas = cv::Scalar(16, 16, 18);
    resized.copyTo(canvas(previewRect));
    cv::rectangle(canvas, previewRect, cv::Scalar(60, 60, 60), 2);

    // Sidebar: display interactive parameter keyboard shortcuts
    int sx = 890;
    cv::putText(canvas, "FILTER-ALL STUDIO", cv::Point(sx, 90), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(100, 255, 100), 2);
    cv::putText(canvas, "Keyboard Controls:", cv::Point(sx, 140), cv::FONT_HERSHEY_PLAIN, 1.3, cv::Scalar(240, 240, 240), 1);
    cv::putText(canvas, "[B / b] Adjust Brightness", cv::Point(sx, 180), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 180), 1);
    cv::putText(canvas, "[C / c] Adjust Contrast",   cv::Point(sx, 220), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 180), 1);
    cv::putText(canvas, "[S / s] Adjust Sharpness",  cv::Point(sx, 260), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 180), 1);
    cv::putText(canvas, "[I] Toggle Inversion",      cv::Point(sx, 300), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 180), 1);
    cv::putText(canvas, "[R] Reset to Default",      cv::Point(sx, 340), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(0, 165, 255), 1);
    cv::putText(canvas, "[ESC] Return to Home",      cv::Point(sx, 400), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(150, 150, 150), 1);
}
