#include "FilterStudioView.h"
#include "CameraChrome.h"
#include "MediaFrameLayout.h"
#include <cstdio>

void FilterStudioView::draw(cv::Mat& canvas, const cv::Mat& currentFrame) {
    if (currentFrame.empty()) return;

    // Process current frame through the manual sandbox filter
    cv::Mat processed = manualFilter.process(currentFrame, 1.0f);
    cv::Rect previewRect(60, 60, 800, 560);
    cv::Mat resized = cropToAspectAndResize(processed, previewRect.size());

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

    char values[128];
    std::snprintf(values, sizeof(values), "B: %+.0f  C: %.1f  S: %.1f",
                  manualFilter.getBrightness(),
                  manualFilter.getContrast(),
                  manualFilter.getSharpness());
    // Showing the live values makes the keyboard controls easier to understand.
    cv::putText(canvas, values, cv::Point(sx, 470), cv::FONT_HERSHEY_PLAIN, 1.35,
                cv::Scalar(100, 255, 100), 1);
    cv::putText(canvas, manualFilter.isInverted() ? "Invert: ON" : "Invert: OFF",
                cv::Point(sx, 505), cv::FONT_HERSHEY_PLAIN, 1.35,
                manualFilter.isInverted() ? cv::Scalar(0, 200, 255) : cv::Scalar(180, 180, 180), 1);

    drawCameraChrome(canvas);
}
