#include "QuadView.h"

QuadView::QuadView() {
    // 2x2 Matrix Slicing Coordinates on a 1280x720 canvas
    quad0 = cv::Rect(0,   0,   640, 360); // Quadrant 0: Top-Left
    quad1 = cv::Rect(640, 0,   640, 360); // Quadrant 1: Top-Right
    quad2 = cv::Rect(0,   360, 640, 360); // Quadrant 2: Bottom-Left
    quad3 = cv::Rect(640, 360, 640, 360); // Quadrant 3: Bottom-Right
}

void QuadView::draw(cv::Mat& canvas, const cv::Mat& currentFrame) {
    if (currentFrame.empty()) return;

    // Resize the input frame once to 640x360 for fast parallel quadrant processing
    cv::Mat halfFrame;
    cv::resize(currentFrame, halfFrame, cv::Size(640, 360));

    // Quadrant 0: Original Unaltered Feed
    halfFrame.copyTo(canvas(quad0));
    cv::putText(canvas, "1. ORIGINAL FEED", cv::Point(20, 35), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

    // Quadrant 1: 1950s Retro Filter
    cv::Mat retroOut = retroFilter.process(halfFrame, 0.85f);
    retroOut.copyTo(canvas(quad1));
    cv::putText(canvas, "2. 1950s RETRO", cv::Point(660, 35), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 200, 255), 2);

    // Quadrant 2: Holiday Warmth Filter
    cv::Mat holidayOut = holidayFilter.process(halfFrame, 0.90f);
    holidayOut.copyTo(canvas(quad2));
    cv::putText(canvas, "3. HOLIDAY WARMTH", cv::Point(20, 395), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 165, 255), 2);

    // Quadrant 3: Party Neon Filter
    cv::Mat partyOut = partyFilter.process(halfFrame, 0.95f);
    partyOut.copyTo(canvas(quad3));
    cv::putText(canvas, "4. PARTY NEON", cv::Point(660, 395), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 100, 255), 2);

    // Draw white divider lines separating the 4 quadrants
    cv::line(canvas, cv::Point(640, 0), cv::Point(640, 720), cv::Scalar(255, 255, 255), 2);
    cv::line(canvas, cv::Point(0, 360), cv::Point(1280, 360), cv::Scalar(255, 255, 255), 2);
}

int QuadView::handleMouseClicked(int x, int y) {
    cv::Point pt(x, y);
    if (quad0.contains(pt)) return 0;
    if (quad1.contains(pt)) return 1;
    if (quad2.contains(pt)) return 2;
    if (quad3.contains(pt)) return 3;
    return -1;
}
