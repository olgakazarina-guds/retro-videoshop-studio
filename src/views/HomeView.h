#pragma once

#include "ofMain.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

// Structure defining each clickable card on the bottom dashboard
struct NavCard {
    ofRectangle bounds;      // Position and size (x, y, width, height)
    std::string title;       // Card title (e.g., "1. Play View")
    std::string line1;       // First description line
    std::string line2;       // Second description line
    std::string actionText;  // Bottom action prompt (e.g., "CLICK TO ENTER >")
    ofColor borderColor;     // Retro accent border color
    int targetState;         // The mode ID to switch to when clicked
};

class HomeView {
public:
    // Constructor
    HomeView();

    // Setup and layout initialization
    void setup();

    // Update animation timers (like the blinking tally light)
    void update();

    // Main draw method: renders the chassis, video frame, and dashboard cards
    void draw();

    // Overloaded draw method if you pass a cv::Mat frame directly
    void draw(const cv::Mat& frame);

    // Handles mouse clicks on the 4 navigation cards (returns target mode ID or 0)
    int handleMouseClick(int x, int y) const;

    // Recalculates card and monitor positions if the window size changes
    void windowResized(int w, int h);

private:
    // Helper drawing functions to keep the code modular and clean
    void drawHeader(float x, float y, float w);
    void drawCameraChassis(float x, float y, float w, float h);
    void drawViewfinderOSD(float x, float y, float w, float h);
    void drawDashboardCards();

    // Screen layout rectangles
    ofRectangle m_monitorRect;           // Area for the video monitor
    std::vector<NavCard> m_cards;        // The 4 dashboard cards

    // Tally light blink animation state
    bool m_tallyBlink;                   // Toggles true/false to create blink effect
    float m_tapeCounter;                 // Simulated mechanical tape counter
};
