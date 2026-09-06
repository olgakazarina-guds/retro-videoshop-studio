#include "HomeView.h"
#include <ctime>

HomeView::HomeView() : frameCounter(0) {
    // 4 clickable deck buttons across the bottom
    btnPlayView     = cv::Rect(40,  475, 265, 185);
    btnSelectMode   = cv::Rect(345, 475, 265, 185);
    btnUploadStream = cv::Rect(650, 475, 265, 185);
    btnManualFilter = cv::Rect(955, 475, 265, 185);
}

void HomeView::draw(cv::Mat& canvas, const cv::Mat& previewFrame) {
    frameCounter++;

    // 1. Dark textured camera body background (deep slate charcoal)
    canvas = cv::Scalar(14, 14, 16);

    // -------------------------------------------------------------------------
    // HARDWARE CHASSIS: Full Camera Bezel Plate
    // -------------------------------------------------------------------------
    cv::Rect screenRect(240, 75, 800, 350);
    cv::Rect outerChassis(215, 20, 850, 425);

    // Solid chassis faceplate with double metallic chamfer border
    cv::rectangle(canvas, outerChassis, cv::Scalar(24, 24, 28), -1);
    cv::rectangle(canvas, outerChassis, cv::Scalar(60, 60, 68), 2);
    cv::rectangle(canvas, cv::Rect(outerChassis.x - 4, outerChassis.y - 4, outerChassis.width + 8, outerChassis.height + 8), 
                 cv::Scalar(40, 40, 46), 1);

    // Chassis Corner Screws (Authentic mechanical detail)
    auto drawScrew = [&](int sx, int sy) {
        cv::circle(canvas, cv::Point(sx, sy), 5, cv::Scalar(45, 45, 50), -1);
        cv::circle(canvas, cv::Point(sx, sy), 5, cv::Scalar(90, 90, 100), 1);
        cv::line(canvas, cv::Point(sx - 3, sy), cv::Point(sx + 3, sy), cv::Scalar(20, 20, 24), 1);
        cv::line(canvas, cv::Point(sx, sy - 3), cv::Point(sx, sy + 3), cv::Scalar(20, 20, 24), 1);
    };
    drawScrew(outerChassis.x + 12, outerChassis.y + 12);
    drawScrew(outerChassis.x + outerChassis.width - 12, outerChassis.y + 12);
    drawScrew(outerChassis.x + 12, outerChassis.y + outerChassis.height - 12);
    drawScrew(outerChassis.x + outerChassis.width - 12, outerChassis.y + outerChassis.height - 12);

    // -------------------------------------------------------------------------
    // TOP BEZEL: Blinking Tally Lamp & Vintage Camera Branding
    // -------------------------------------------------------------------------
    bool recBlink = (frameCounter / 15) % 2 == 0;
    cv::Point tallyCenter(255, 48);

    // Rec LED ring and lamp
    cv::circle(canvas, tallyCenter, 9, cv::Scalar(75, 75, 85), 2);
    if (recBlink) {
        cv::circle(canvas, tallyCenter, 7, cv::Scalar(0, 0, 245), -1);
        cv::putText(canvas, "REC TALLY", cv::Point(272, 53), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(0, 0, 245), 2);
    } else {
        cv::circle(canvas, tallyCenter, 7, cv::Scalar(0, 0, 75), -1);
        cv::putText(canvas, "REC TALLY", cv::Point(272, 53), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(110, 110, 120), 1);
    }

    // Camera Brand Badge in center
    cv::Rect badgeRect(520, 36, 240, 22);
    cv::rectangle(canvas, badgeRect, cv::Scalar(32, 32, 38), -1);
    cv::rectangle(canvas, badgeRect, cv::Scalar(80, 80, 90), 1);
    cv::putText(canvas, "RETRO-CAM // V-800", cv::Point(535, 52), 
               cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(210, 210, 220), 1);

    // Right-hand format telemetry
    cv::putText(canvas, "NTSC 525-L | 30 FPS", cv::Point(870, 53),
               cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(150, 150, 160), 1);

    // -------------------------------------------------------------------------
    // LIVE VIEWFINDER SCREEN
    // -------------------------------------------------------------------------
    if (!previewFrame.empty()) {
        cv::Mat resizedPreview;
        cv::resize(previewFrame, resizedPreview, screenRect.size());
        resizedPreview.copyTo(canvas(screenRect));
    } else {
        cv::rectangle(canvas, screenRect, cv::Scalar(5, 5, 5), -1);
    }
    // Inner bevel border around the video screen
    cv::rectangle(canvas, screenRect, cv::Scalar(60, 60, 65), 3);

    // -------------------------------------------------------------------------
    // OPTICAL OVERLAY: Corner Brackets & Center Crosshair
    // -------------------------------------------------------------------------
    int pad = 15;
    int bLen = 16;
    cv::Scalar bracketCol(0, 190, 255); // Amber optical brackets

    // Top-Left [
    cv::line(canvas, cv::Point(screenRect.x + pad, screenRect.y + pad), cv::Point(screenRect.x + pad + bLen, screenRect.y + pad), bracketCol, 2);
    cv::line(canvas, cv::Point(screenRect.x + pad, screenRect.y + pad), cv::Point(screenRect.x + pad, screenRect.y + pad + bLen), bracketCol, 2);
    // Top-Right ]
    cv::line(canvas, cv::Point(screenRect.x + screenRect.width - pad, screenRect.y + pad), cv::Point(screenRect.x + screenRect.width - pad - bLen, screenRect.y + pad), bracketCol, 2);
    cv::line(canvas, cv::Point(screenRect.x + screenRect.width - pad, screenRect.y + pad), cv::Point(screenRect.x + screenRect.width - pad, screenRect.y + pad + bLen), bracketCol, 2);
    // Bottom-Left [
    cv::line(canvas, cv::Point(screenRect.x + pad, screenRect.y + screenRect.height - pad), cv::Point(screenRect.x + pad + bLen, screenRect.y + screenRect.height - pad), bracketCol, 2);
    cv::line(canvas, cv::Point(screenRect.x + pad, screenRect.y + screenRect.height - pad), cv::Point(screenRect.x + pad, screenRect.y + screenRect.height - pad - bLen), bracketCol, 2);
    // Bottom-Right ]
    cv::line(canvas, cv::Point(screenRect.x + screenRect.width - pad, screenRect.y + screenRect.height - pad), cv::Point(screenRect.x + screenRect.width - pad - bLen, screenRect.y + screenRect.height - pad), bracketCol, 2);
    cv::line(canvas, cv::Point(screenRect.x + screenRect.width - pad, screenRect.y + screenRect.height - pad), cv::Point(screenRect.x + screenRect.width - pad, screenRect.y + screenRect.height - pad - bLen), bracketCol, 2);

    // Subtle Center Crosshair
    int cx = screenRect.x + screenRect.width / 2;
    int cy = screenRect.y + screenRect.height / 2;
    cv::line(canvas, cv::Point(cx - 12, cy), cv::Point(cx + 12, cy), cv::Scalar(240, 240, 240), 1);
    cv::line(canvas, cv::Point(cx, cy - 12), cv::Point(cx, cy + 12), cv::Scalar(240, 240, 240), 1);
    cv::circle(canvas, cv::Point(cx, cy), 5, cv::Scalar(240, 240, 240), 1);

    // -------------------------------------------------------------------------
    // REAL-TIME TIMESTAMP OSD
    // -------------------------------------------------------------------------
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char timeBuffer[64];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%b %d %Y  %I:%M:%S %p", localTime);

    cv::Rect timeBox(screenRect.x + 18, screenRect.y + screenRect.height - 36, 320, 24);
    cv::rectangle(canvas, timeBox, cv::Scalar(0, 0, 0), -1);
    cv::rectangle(canvas, timeBox, cv::Scalar(0, 140, 230), 1);
    cv::putText(canvas, timeBuffer, cv::Point(timeBox.x + 8, timeBox.y + 17), 
               cv::FONT_HERSHEY_PLAIN, 1.15, cv::Scalar(0, 180, 255), 2);

    // -------------------------------------------------------------------------
    // BOTTOM CHASSIS BAR: Camcorder Telemetry
    // -------------------------------------------------------------------------
    int botY = screenRect.y + screenRect.height + 26;
    cv::putText(canvas, "BATT [||||] 92%", cv::Point(245, botY), cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(80, 220, 120), 1);
    cv::putText(canvas, "TAPE COUNTER: 0142", cv::Point(440, botY), cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(170, 170, 180), 1);
    cv::putText(canvas, "AUDIO: [||||||..] -12dB", cv::Point(670, botY), cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(0, 200, 255), 1);
    cv::putText(canvas, "IRIS: f/2.8", cv::Point(930, botY), cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(170, 170, 180), 1);

    // -------------------------------------------------------------------------
    // DECK CONTROL CARDS (Physical Tactile Keys)
    // -------------------------------------------------------------------------
    auto drawCard = [&](const cv::Rect& r, const std::string& title, 
                        const std::string& sub1, const std::string& sub2, 
                        cv::Scalar accentColor) {
        // Dark tactile button body
        cv::rectangle(canvas, r, cv::Scalar(26, 26, 30), -1);
        cv::rectangle(canvas, r, accentColor, 2);

        // Top edge highlight line (gives 3D physical key bevel)
        cv::line(canvas, cv::Point(r.x + 2, r.y + 2), cv::Point(r.x + r.width - 2, r.y + 2), cv::Scalar(75, 75, 85), 2);

        // Card Title
        cv::putText(canvas, title, cv::Point(r.x + 16, r.y + 34), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.62, cv::Scalar(245, 245, 245), 2);

        // Subtitles (independent lines to avoid any '?' symbol issues)
        cv::putText(canvas, sub1, cv::Point(r.x + 16, r.y + 70), 
                   cv::FONT_HERSHEY_PLAIN, 1.15, cv::Scalar(170, 170, 175), 1);
        cv::putText(canvas, sub2, cv::Point(r.x + 16, r.y + 98), 
                   cv::FONT_HERSHEY_PLAIN, 1.15, cv::Scalar(170, 170, 175), 1);

        // Action prompt
        cv::putText(canvas, "CLICK TO ENTER >", cv::Point(r.x + 16, r.y + 155), 
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
