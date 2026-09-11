#pragma once

// Core openFrameworks header (handles window, keyboard, and OpenGL graphics)
#include "ofMain.h"

// Project headers
#include "MediaSourceManager.h"
#include "views/HomeView.h"
#include "views/QuadView.h"
#include "views/ModeView.h"
#include "views/FilterStudioView.h"
#include "filters/RetroFilter.h"
#include "filters/HolidayFilter.h"
#include "filters/PartyFilter.h"

// State machine enum: tracks which screen the user is currently viewing
enum class AppState {
    HOME,          // Main camera chassis & dashboard
    QUAD_VIEW,     // 2x2 split comparison screen
    MODE_VIEW,     // Single filter focus view with intensity slider
    FILTER_STUDIO  // Manual DSP sandbox with keyboard controls
};

class ofApp : public ofBaseApp {
private:
    MediaSourceManager mediaManager;  // Loads images, videos, or webcam feeds
    AppState currentState;            // Currently active screen state

    // View screen instances
    HomeView homeView;
    QuadView quadView;
    ModeView modeView;
    FilterStudioView studioView;

    // Filter instances for routing
    RetroFilter retroFilter;
    HolidayFilter holidayFilter;
    PartyFilter partyFilter;

    // Manual Filter keyboard state
    float manualBrightness = 0.0f;
    float manualContrast = 1.0f;
    float manualSharpness = 0.0f;
    bool manualInvert = false;

    // cv::Mat: OpenCV CPU memory buffer where pixels and UI are drawn
    cv::Mat frameBuffer;

    // ofImage: openFrameworks OpenGL texture used to send pixels to your GPU/screen
    ofImage displayImage;

	//Mutex to thread-safe access between update() and draw() methods
	ofMutex mutex;


public:
    // Core openFrameworks lifecycle methods
    void setup() override;
    void update() override;
    void draw() override;

    // User input event listeners
    void keyPressed(int key) override;
    void mousePressed(int x, int y, int button) override;
};
