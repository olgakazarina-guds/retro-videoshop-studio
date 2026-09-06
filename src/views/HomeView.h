#pragma once

#include "ofMain.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

// Enum representing the user's action when clicking a card
enum class HomeAction {
    NONE = 0,
    PLAY_VIEW,       // Option 1: 2x2 Quad Matrix
    SELECT_MODE,     // Option 2: Select Mode
    UPLOAD_STREAM,   // Option 3: Upload/Stream
    MANUAL_FILTER    // Option 4: Manual Filter Studio
};

// Structure defining each card on the bottom dashboard
struct NavCard {
    ofRectangle bounds;
    std::string title;
    std::string line1;
    std::string line2;
    std::string actionText;
    ofColor borderColor;
    HomeAction action;
};

class HomeView {
public:
    HomeView();

    void setup();
    void update();

    // Draw methods supporting both empty call and with an image/cv::Mat frame
    void draw();
    void draw(const cv::Mat& frame);
    void draw(const ofImage& img);
    void draw(const ofTexture& tex);

    // Matches the exact method name Xcode is looking for:
    HomeAction handleMouseClicked(int x, int y) const;

    void windowResized(int w, int h);

private:
    void drawHeader(float x, float y, float w);
    void drawCameraChassis(float x, float y, float w, float h);
    void drawViewfinderOSD(float x, float y, float w, float h);
    void drawDashboardCards();

    ofRectangle m_monitorRect;
    std::vector<NavCard> m_cards;

    bool m_tallyBlink;
    float m_tapeCounter;
};
