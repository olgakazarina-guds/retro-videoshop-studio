#include "QuadView.h"
#include "CameraChrome.h"
#include "MediaFrameLayout.h"
#include <string>

QuadView::QuadView() {
    // Leave a camera-style top rail and a narrow lower rail around the matrix.
    quad0 = cv::Rect(28, 72, 612, 320); // Quadrant 0: Top-Left
    quad1 = cv::Rect(640, 72, 612, 320); // Quadrant 1: Top-Right
    // Leave a lower camera rail for the shared rotation controls.
    quad2 = cv::Rect(28, 400, 612, 270); // Quadrant 2: Bottom-Left
    quad3 = cv::Rect(640, 400, 612, 270); // Quadrant 3: Bottom-Right
}

void QuadView::draw(cv::Mat& canvas, const cv::Mat& currentFrame) {
    if (currentFrame.empty()) return;

    canvas = cv::Scalar(18, 18, 20);

    auto drawPanel = [&](const cv::Rect& panel, const cv::Mat& image,
                         const std::string& title, const cv::Scalar& color) {
        cv::rectangle(canvas, panel, cv::Scalar(28, 28, 32), -1);
        cv::rectangle(canvas, panel, cv::Scalar(95, 95, 105), 1);

        const int titleHeight = 42;
        cv::Rect imageRect(panel.x + 1, panel.y + titleHeight,
                           panel.width - 2, panel.height - titleHeight - 1);
        cv::Mat resized = cropToAspectAndResize(image, imageRect.size());
        resized.copyTo(canvas(imageRect));

        cv::line(canvas, cv::Point(panel.x, panel.y + titleHeight),
                 cv::Point(panel.x + panel.width, panel.y + titleHeight),
                 cv::Scalar(95, 95, 105), 1);
        drawCameraBadge(canvas, title,
                        cv::Point(panel.x + 12, panel.y + 29), color, 0.58);
    };

    // Quadrant 0: Original Unaltered Feed
    drawPanel(quad0, currentFrame, "1. ORIGINAL FEED", cv::Scalar(245, 245, 245));

    // Quadrant 1: 1950s Retro Filter
    cv::Mat retroOut = retroFilter.process(currentFrame, 0.85f);
    drawPanel(quad1, retroOut, "2. 1950s RETRO", cv::Scalar(0, 200, 255));

    // Quadrant 2: Holiday Warmth Filter
    cv::Mat holidayOut = holidayFilter.process(currentFrame, 0.90f);
    drawPanel(quad2, holidayOut, "3. HOLIDAY WARMTH", cv::Scalar(0, 185, 255));

    // Quadrant 3: Party Neon Filter
    cv::Mat partyOut = partyFilter.process(currentFrame, 0.95f);
    drawPanel(quad3, partyOut, "4. PARTY NEON", cv::Scalar(255, 155, 255));

    drawCameraChrome(canvas);
}

int QuadView::handleMouseClicked(int x, int y) {
    cv::Point pt(x, y);
    if (quad0.contains(pt)) return 0;
    if (quad1.contains(pt)) return 1;
    if (quad2.contains(pt)) return 2;
    if (quad3.contains(pt)) return 3;
    return -1;
}
