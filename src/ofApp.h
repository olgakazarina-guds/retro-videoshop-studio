#pragma once

#include "ofMain.h"
#include "MediaSourceManager.h"
#include "views/HomeView.h"
#include "views/QuadView.h"
#include "views/ModeView.h"
#include "views/FilterStudioView.h"
#include "filters/RetroFilter.h"
#include "filters/HolidayFilter.h"
#include "filters/PartyFilter.h"

// State machine enum for application navigation
enum class AppState {
    HOME,
    QUAD_VIEW,
    MODE_VIEW,
    FILTER_STUDIO
};

class ofApp : public ofBaseApp {
private:
    MediaSourceManager mediaManager; // Handles webcam/video/image input
    AppState currentState;           // Active application state

    // Views
    HomeView homeView;
    QuadView quadView;
    ModeView modeView;
    FilterStudioView studioView;

    // Filters for mode routing
    RetroFilter retroFilter;
    HolidayFilter holidayFilter;
    PartyFilter partyFilter;

    cv::Mat frameBuffer; // Canvas matrix where active view is rendered

public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    void mousePressed(int x, int y, int button) override;
};
