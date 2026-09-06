#include "HomeView.h"
#include <ctime> // C++ standard library for real-time system clock

// ==============================================================================
// Constructor: Calculate coordinates for the 4 bottom cards on 1280x720 canvas
// ==============================================================================
HomeView::HomeView() : frameCounter(0) {
    // 4 cards spaced evenly with 40px left margin and 40px gaps
    btnPlayView     = cv::Rect(40,  460, 265, 200);
    btnSelectMode   = cv::Rect(345, 460, 265, 200);
    btnUploadStream = cv::Rect(650, 460, 265, 200);
    btnManualFilter = cv::Rect(955, 460, 265, 200);
}

// ==============================================================================
// Draw: Render camera chassis, live viewfinder, tally light, and cards
// ==============================================================================
void HomeView::draw(cv::Mat& canvas, const cv::Mat& previewFrame) {
    frameCounter++;

    // 1. Fill entire canvas with dark matte vintage chassis tone (BGR: 18, 18, 20)
    canvas = cv::Scalar(18, 18, 20);

    // 2. Define the centered 16:9 viewfinder monitor area
    cv::Rect screenRect(240, 70, 800, 360);

    // 3. Draw outer camera chassis bezel plate around the screen
    cv::Rect outerChassis(225, 22, 830, 420);
    cv::rectangle(canvas, outerChassis, cv::Scalar(28, 28, 34), -1); // Dark filled plate
    cv::rectangle(canvas, outerChassis, cv::Scalar(65, 65, 75), 2);  // Metallic edge outline

    // -------------------------------------------------------------------------
    // 4. Blinking [REC] Tally Lamp (Calm 1-second pulse, OUTSIDE the video area)
    // -------------------------------------------------------------------------
    // At ~60 FPS, 45 frames on / 45 frames off gives a calm, natural breathing rhythm
    bool recBlink = (frameCounter / 45) % 2 == 0;
    cv::Point tallyCenter(260, 46); // Y=46 is in the chassis, above the video (Y=70)

    // Outer lamp chrome ring
    cv::circle(canvas, tallyCenter, 8, cv::Scalar(70, 70, 80), 2);

    if (recBlink) {
        // Bright glowing red LED dot
        cv::circle(canvas, tallyCenter, 6, cv::Scalar(0, 0, 240), -1);
        cv::putText(canvas, "REC", cv::Point(276, 52), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(0, 0, 240), 2);
    } else {
        // Dim standby red LED dot
        cv::circle(canvas, tallyCenter, 6, cv::Scalar(0, 0, 75), -1);
        cv::putText(canvas, "REC", cv::Point(276, 52), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(110, 110, 120), 1);
    }

    // -------------------------------------------------------------------------
    // 5. Render Centered Live Video Frame
    // -------------------------------------------------------------------------
    if (!previewFrame.empty()) {
        cv::Mat resizedPreview;
        // Scale input frame to fit the 800x360 monitor rectangle
        cv::resize(previewFrame, resizedPreview, screenRect.size());
        resizedPreview.copyTo(canvas(screenRect));
    } else {
        // Fallback dark screen if feed is loading
        cv::rectangle(canvas, screenRect, cv::Scalar(5, 5, 5), -1);
    }
    // Clean screen border
    cv::rectangle(canvas, screenRect, cv::Scalar(80, 80, 85), 2);

    // -------------------------------------------------------------------------
    // 6. Viewfinder Center Crosshair
    // -------------------------------------------------------------------------
    int cx = screenRect.x + screenRect.width / 2;
    int cy = screenRect.y + screenRect.height / 2;
    cv::line(canvas, cv::Point(cx - 10, cy), cv::Point(cx + 10, cy), cv::Scalar(220, 220, 220), 1);
    cv::line(canvas, cv::Point(cx, cy - 10), cv::Point(cx, cy + 10), cv::Scalar(220, 220, 220), 1);
    cv::circle(canvas, cv::Point(cx, cy), 5, cv::Scalar(220, 220, 220), 1);

    // -------------------------------------------------------------------------
    // 7. Live Real-Time Date & Time Stamp (Replaces hardcoded 1952 date)
    // -------------------------------------------------------------------------
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char timeBuffer[64];
    // Format: "Sep 06 2026  12:57:53 PM"
    std::strftime(timeBuffer, sizeof(timeBuffer), "%b %d %Y  %I:%M:%S %p", localTime);

    // Dark protective background box so timestamp is readable over any footage
    cv::Rect timeBox(screenRect.x + 15, screenRect.y + screenRect.height - 35, 330, 26);
    cv::rectangle(canvas, timeBox, cv::Scalar(0, 0, 0), -1);
    cv::rectangle(canvas, timeBox, cv::Scalar(0, 140, 230), 1);

    // Amber vintage phosphor text
    cv::putText(canvas, timeBuffer, cv::Point(timeBox.x + 8, timeBox.y + 18), 
               cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(0, 180, 255), 2);

    // -------------------------------------------------------------------------
    // 8. Lambda Function to Draw Dashboard Cards
    // (Notice sub1 and sub2 are drawn on separate lines to prevent '?' bugs!)
    // -------------------------------------------------------------------------
    auto drawCard = [&](const cv::Rect& r, const std::string& title, 
                        const std::string& sub1, const std::string& sub2, 
                        cv::Scalar accentColor) {
        // Dark card body & colored border
        cv::rectangle(canvas, r, cv::Scalar(30, 30, 36), -1);
        cv::rectangle(canvas, r, accentColor, 2);

        // Card Title
        cv::putText(canvas, title, cv::Point(r.x + 16, r.y + 36), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(245, 245, 245), 2);

        // Subtitle Line 1
        cv::putText(canvas, sub1, cv::Point(r.x + 16, r.y + 75), 
                   cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 185), 1);

        // Subtitle Line 2
        cv::putText(canvas, sub2, cv::Point(r.x + 16, r.y + 105), 
                   cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 185), 1);

        // Action Prompt at bottom
        cv::putText(canvas, "CLICK TO ENTER >", cv::Point(r.x + 16, r.y + 165), 
                   cv::FONT_HERSHEY_PLAIN, 1.15, accentColor, 1);
    };

    // Draw the 4 interactive cards with clean line breaks
    drawCard(btnPlayView,     "1. Play View",     "2x2 Quad Matrix",          "Live 4-way stream",      cv::Scalar(0, 200, 255));
    drawCard(btnSelectMode,   "2. Select Mode",   "Retro / Holiday / Party",  "Intensity Scale Studio", cv::Scalar(60, 60, 240));
    drawCard(btnUploadStream, "3. Upload/Stream", "Load Image or Video",      "Webcam Configuration",   cv::Scalar(240, 200, 0));
    drawCard(btnManualFilter, "4. Manual Filter", "Filter-All Sandbox",       "Raw DSP Sliders",        cv::Scalar(100, 220, 100));
}

// ==============================================================================
// Handle Mouse Click: Returns which card was clicked using cv::Rect::contains()
// ==============================================================================
HomeAction HomeView::handleMouseClicked(int x, int y) {
    cv::Point pt(x, y);
    if (btnPlayView.contains(pt))     return HomeAction::PLAY_VIEW;
    if (btnSelectMode.contains(pt))   return HomeAction::SELECT_MODE;
    if (btnUploadStream.contains(pt)) return HomeAction::UPLOAD_STREAM;
    if (btnManualFilter.contains(pt)) return HomeAction::MANUAL_FILTER;
    return HomeAction::NONE;
}
