#include "HomeView.h"

HomeView::HomeView() : frameCounter(0) {
    // Define the bounding rectangles (x, y, width, height) for the 4 clickable cards on a 1280x720 canvas
    btnPlayView     = cv::Rect(40,  460, 260, 180);
    btnSelectMode   = cv::Rect(340, 460, 260, 180);
    btnUploadStream = cv::Rect(640, 460, 260, 180);
    btnManualFilter = cv::Rect(940, 460, 260, 180);
}

void HomeView::draw(cv::Mat& canvas, const cv::Mat& previewFrame) {
    frameCounter++;

    // Fill background with a dark vintage chassis color (BGR: 18, 18, 20)
    canvas = cv::Scalar(18, 18, 20);

    // STEP 1: Render Centered Live Viewfinder Screen
    cv::Rect screenRect(240, 70, 800, 360);
    if (!previewFrame.empty()) {
        cv::Mat resizedPreview;
        cv::resize(previewFrame, resizedPreview, screenRect.size());
        resizedPreview.copyTo(canvas(screenRect));
    }
    // Draw outer metallic monitor border
    cv::rectangle(canvas, screenRect, cv::Scalar(80, 80, 80), 3);

    // STEP 2: Pulsing Red [REC] Indicator (blinks on and off every 15 frames)
    bool recBlink = (frameCounter / 15) % 2 == 0;
    if (recBlink) {
        // Draw solid red recording dot
        cv::circle(canvas, cv::Point(265, 95), 8, cv::Scalar(0, 0, 230), -1);
    }
    cv::putText(canvas, "REC", cv::Point(280, 102), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 230), 2);

    // STEP 3: Digital Orange Retro Timestamp (CAM-1 1952-06-14)
    cv::putText(canvas, "CAM-1  1952-06-14  14:32:00", cv::Point(260, 410), 
               cv::FONT_HERSHEY_PLAIN, 1.4, cv::Scalar(0, 165, 255), 2);

    // STEP 4: Viewfinder Alignment Crosshair in center
    cv::line(canvas, cv::Point(630, 250), cv::Point(650, 250), cv::Scalar(200, 200, 200), 1);
    cv::line(canvas, cv::Point(640, 240), cv::Point(640, 260), cv::Scalar(200, 200, 200), 1);

    // STEP 5: Lambda helper to render each interactive dashboard card
    auto drawCard = [&](const cv::Rect& r, const std::string& title, const std::string& subtitle, cv::Scalar accentColor) {
        cv::rectangle(canvas, r, cv::Scalar(32, 32, 36), -1); // Dark card background
        cv::rectangle(canvas, r, accentColor, 2);              // 2px accent outline
        cv::putText(canvas, title, cv::Point(r.x + 15, r.y + 40), cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(240, 240, 240), 2);
        cv::putText(canvas, subtitle, cv::Point(r.x + 15, r.y + 80), cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(160, 160, 160), 1);
        cv::putText(canvas, "CLICK TO ENTER >", cv::Point(r.x + 15, r.y + 150), cv::FONT_HERSHEY_PLAIN, 1.0, accentColor, 1);
    };

    // Draw the 4 clickable interactive zones
    drawCard(btnPlayView,     "1. Play View",     "2x2 Quad Matrix\nLive simultaneous", cv::Scalar(0, 200, 255));
    drawCard(btnSelectMode,   "2. Select Mode",   "Retro / Holiday / Party\nIntensity Scale", cv::Scalar(60, 60, 240));
    drawCard(btnUploadStream, "3. Upload/Stream", "Load Image/Video\nWebcam Config", cv::Scalar(240, 200, 0));
    drawCard(btnManualFilter, "4. Manual Filter", "Filter-All Sandbox\nRaw Sliders", cv::Scalar(100, 220, 100));
}

HomeAction HomeView::handleMouseClicked(int x, int y) {
    cv::Point pt(x, y);
    // Check if mouse click coordinates fall inside any button rectangle
    if (btnPlayView.contains(pt))     return HomeAction::PLAY_VIEW;
    if (btnSelectMode.contains(pt))   return HomeAction::SELECT_MODE;
    if (btnUploadStream.contains(pt)) return HomeAction::UPLOAD_STREAM;
    if (btnManualFilter.contains(pt)) return HomeAction::MANUAL_FILTER;
    return HomeAction::NONE;
}
