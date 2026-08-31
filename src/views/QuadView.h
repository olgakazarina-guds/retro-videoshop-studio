#pragma once

#include <opencv2/opencv.hpp>
#include "../filters/RetroFilter.h"
#include "../filters/HolidayFilter.h"
#include "../filters/PartyFilter.h"

// QuadView uses Composition ("Has-A" relationship):
// It manages instances of RetroFilter, HolidayFilter, and PartyFilter to process 4 quadrants simultaneously.
class QuadView {
private:
    RetroFilter retroFilter;
    HolidayFilter holidayFilter;
    PartyFilter partyFilter;

    // 4 quadrant regions sliced using Region of Interest (cv::Rect)
    cv::Rect quad0; // Top-Left: Original
    cv::Rect quad1; // Top-Right: 1950s Retro
    cv::Rect quad2; // Bottom-Left: Holiday Warmth
    cv::Rect quad3; // Bottom-Right: Party Neon

public:
    QuadView();
    ~QuadView() = default;

    // Renders the 2x2 multi-view matrix onto the canvas
    void draw(cv::Mat& canvas, const cv::Mat& currentFrame);

    // Hit-testing: returns 0, 1, 2, or 3 depending on which quadrant was clicked (-1 if none)
    int handleMouseClicked(int x, int y);
};
