#include "HomeView.h"
#include <ctime>

HomeView::HomeView() : frameCounter(0) {
    // 4 clickable cards across the bottom of the 1280x720 canvas
    btnPlayView     = cv::Rect(40,  460, 265, 200);
    btnSelectMode   = cv::Rect(345, 460, 265, 200);
    btnUploadStream = cv::Rect(650, 460, 265, 200);
    btnManualFilter = cv::Rect(955, 460, 265, 200);
}

void HomeView::draw(cv::Mat& canvas, const cv::Mat& previewFrame) {
    frameCounter++;

    // 1. Fill canvas background with vintage dark chassis tone
    canvas = cv::Scalar(18, 18, 20);

    // 2. Centered viewfinder monitor dimensions
    cv::Rect screenRect(240, 70, 800, 360);

    // Outer camera bezel chassis
    cv::Rect outerChassis(230, 25, 820, 415);
    cv::rectangle(canvas, outerChassis, cv::Scalar(28, 28, 34), -1);
    cv::rectangle(canvas, outerChassis, cv::Scalar(60, 60, 68), 2);

    // -------------------------------------------------------------------------
    // 3. Blinking [REC] Tally Lamp in Top Bezel (OUTSIDE the video area)
    // -------------------------------------------------------------------------
    bool recBlink = (frameCounter / 15) % 2 == 0;
    cv::Point tallyCenter(265, 48); // Above screenRect (Y=48 vs Screen Y=70)

    cv::circle(canvas, tallyCenter, 9, cv::Scalar(70, 70, 80), 2);
    if (recBlink) {
        cv::circle(canvas, tallyCenter, 7, cv::Scalar(0, 0, 240), -1); // Glowing red
        cv::putText(canvas, "REC TALLY", cv::Point(282, 53), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 240), 2);
    } else {
        cv::circle(canvas, tallyCenter, 7, cv::Scalar(0, 0, 80), -1);  // Standby dark red
        cv::putText(canvas, "REC TALLY", cv::Point(282, 53), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(120, 120, 130), 1);
    }

    cv::putText(canvas, "CAM-1 OPTICAL | NTSC 1280x720", cv::Point(780, 53),
               cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(160, 160, 170), 1);

    // -------------------------------------------------------------------------
    // 4. Render Live Video Feed
    // -------------------------------------------------------------------------
    if (!previewFrame.empty()) {
        cv::Mat resizedPreview;
        cv::resize(previewFrame, resizedPreview, screenRect.size());
        resizedPreview.copyTo(canvas(screenRect));
    } else {
        cv::rectangle(canvas, screenRect, cv::Scalar(5, 5, 5), -1);
    }
    cv::rectangle(canvas, screenRect, cv::Scalar(80, 80, 80), 2);

    // -------------------------------------------------------------------------
    // 5. Optical Center Crosshair
    // -------------------------------------------------------------------------
    cv::line(canvas, cv::Point(630, 250), cv::Point(650, 250), cv::Scalar(220, 220, 220), 1);
    cv::line(canvas, cv::Point(640, 240), cv::Point(640, 260), cv::Scalar(220, 220, 220), 1);
    cv::circle(canvas, cv::Point(640, 250), 6, cv::Scalar(220, 220, 220), 1);

    // -------------------------------------------------------------------------
    // 6. Real-Time Dynamic Date & Time Stamp (No more hardcoded 1952 date)
    // -------------------------------------------------------------------------
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char timeBuffer[64];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%b %d %Y  %I:%M:%S %p", localTime);

    cv::Rect timeBox(screenRect.x + 15, screenRect.y + screenRect.height - 35, 330, 26);
    cv::rectangle(canvas, timeBox, cv::Scalar(0, 0, 0), -1);
    cv::rectangle(canvas, timeBox, cv::Scalar(0, 140, 230), 1);
    cv::putText(canvas, timeBuffer, cv::Point(timeBox.x + 8, timeBox.y + 18), 
               cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(0, 180, 255), 2);

    // -------------------------------------------------------------------------
    // 7. Dashboard Cards (Separate lines to prevent '?' character encoding bugs)
    // -------------------------------------------------------------------------
    auto drawCard = [&](const cv::Rect& r, const std::string& title, 
                        const std::string& sub1, const std::string& sub2, 
                        cv::Scalar accentColor) {
        cv::rectangle(canvas, r, cv::Scalar(30, 30, 36), -1);
        cv::rectangle(canvas, r, accentColor, 2);

        cv::putText(canvas, title, cv::Point(r.x + 16, r.y + 36), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(245, 245, 245), 2);
        cv::putText(canvas, sub1, cv::Point(r.x + 16, r.y + 75), 
                   cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 185), 1);
        cv::putText(canvas, sub2, cv::Point(r.x + 16, r.y + 105), 
                   cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(180, 180, 185), 1);
        cv::putText(canvas, "CLICK TO ENTER >", cv::Point(r.x + 16, r.y + 165), 
                   cv::FONT_HERSHEY_PLAIN, 1.15, accentColor, 1);
    };

    drawCard(btnPlayView,     "1. Play View",     "2x2 Quad Matrix",          "Live 4-way stream",      cv::Scalar(0, 200, 255));
    drawCard(btnSelectMode,   "2. Select Mode",   "Retro / Holiday / Party",  "Intensity Scale Studio", cv::Scalar(60, 60, 240));
    drawCard(btnUploadStream, "3. Upload/Stream", "Load Image or Video",      "Webcam Configuration",   cv::Scalar(240, 200, 0));
    drawCard(btnManualFilter, "4. Manual Filter", "Filter-All Sandbox",       "Raw DSP Sliders",        cv::Scalar(100, 220, 100));
}

HomeAction HomeView::handleMouseClicked(int x, int y) {
    cv::Point pt(x, y);
    if (btnPlayView.contains(pt))     return HomeAction::PLAY_VIEW;
    if (btnSelectMode.contains(pt))   return HomeAction::SELECT_MODE;
    if (btnUploadStream.contains(pt)) return HomeAction::UPLOAD_STREAM;
    if (btnManualFilter.contains(pt)) return HomeAction::MANUAL_FILTER;
    return HomeAction::NONE;
}
