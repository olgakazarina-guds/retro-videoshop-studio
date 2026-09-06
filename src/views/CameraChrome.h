#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Shared camera-body treatment used by every screen so navigation feels like
// changing controls on one physical vintage camera.
inline void drawCameraChrome(cv::Mat& canvas) {
    const cv::Scalar body(18, 18, 20);
    const cv::Scalar metal(72, 72, 80);

    cv::rectangle(canvas, cv::Rect(8, 8, canvas.cols - 16, canvas.rows - 16), body, 4);
    cv::rectangle(canvas, cv::Rect(20, 20, canvas.cols - 40, canvas.rows - 40), metal, 1);

    const cv::Point screws[] = {
        {30, 30},
        {canvas.cols - 30, 30},
        {30, canvas.rows - 30},
        {canvas.cols - 30, canvas.rows - 30}
    };
    for (const cv::Point& screw : screws) {
        cv::circle(canvas, screw, 7, cv::Scalar(55, 55, 62), -1);
        cv::circle(canvas, screw, 7, metal, 1);
        cv::line(canvas, screw - cv::Point(3, 3), screw + cv::Point(3, 3), metal, 1);
        cv::line(canvas, screw + cv::Point(3, -3), screw - cv::Point(3, -3), metal, 1);
    }

    cv::circle(canvas, cv::Point(58, 48), 6, cv::Scalar(0, 0, 210), -1);
    cv::putText(canvas, "REC", cv::Point(72, 53),
                cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(0, 0, 210), 1);
}

inline void drawCameraBadge(cv::Mat& canvas, const std::string& text,
                            const cv::Point& origin, const cv::Scalar& color,
                            double scale = 0.7) {
    int baseline = 0;
    const cv::Size textSize = cv::getTextSize(
        text, cv::FONT_HERSHEY_SIMPLEX, scale, 2, &baseline);
    const cv::Rect badge(origin.x - 8, origin.y - textSize.height - 10,
                         textSize.width + 16, textSize.height + baseline + 14);

    cv::rectangle(canvas, badge, cv::Scalar(18, 18, 20), -1);
    cv::rectangle(canvas, badge, cv::Scalar(95, 95, 105), 1);
    cv::putText(canvas, text, origin, cv::FONT_HERSHEY_SIMPLEX,
                scale, cv::Scalar(10, 10, 12), 4);
    cv::putText(canvas, text, origin, cv::FONT_HERSHEY_SIMPLEX,
                scale, color, 2);
}
