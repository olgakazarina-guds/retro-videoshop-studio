#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Shared camera-body treatment used by every screen so navigation feels like
// changing controls on one physical vintage camera.
inline void drawCameraChrome(cv::Mat& canvas, const std::string& label) {
    const cv::Scalar body(18, 18, 20);
    const cv::Scalar metal(72, 72, 80);
    const cv::Scalar text(185, 185, 195);

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
    cv::putText(canvas, label, cv::Point(canvas.cols - 260, 52),
                cv::FONT_HERSHEY_PLAIN, 1.2, text, 1);
}
