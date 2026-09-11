#include "ofApp.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr int kCanvasWidth = 1280;
constexpr int kCanvasHeight = 720;
}

// ==============================================================================
// SETUP: Called once when the application starts
// ==============================================================================
void ofApp::setup() {
    // 1. Set window title and standard 16:9 widescreen dimensions
    ofSetWindowTitle("Retro Videoshop Studio");
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    // 2. Prevent the ESC key from closing the app, so we can use it to return home
    ofSetEscapeQuitsApp(false);

    // 3. Start on the Home dashboard
    currentState = AppState::HOME;

    // 4. Start the manual studio with neutral filter values
    studioView.getFilter().reset();

    // 5. Create an empty black 1280x720 canvas in OpenCV (8-bit, 3 color channels: BGR)
    frameBuffer = cv::Mat::zeros(720, 1280, CV_8UC3);

    // 6. Pre-allocate openFrameworks GPU texture memory to prevent lag during drawing
    displayImage.allocate(1280, 720, OF_IMAGE_COLOR);

    // 7. Load default test media (falls back to a vintage color-bar pattern if missing)
    mediaManager.loadImage("Test.jpg");
}

// ==============================================================================
// UPDATE: Called continuously to calculate logic and process video frames
// ==============================================================================
void ofApp::update() {
    // Advance video playback before rendering the current frame.
    mediaManager.update();

    // Get the current video or photo frame from our media manager
    cv::Mat currentFrame = mediaManager.getCurrentFrame();

    // Delegate rendering to whichever screen is currently active
    switch (currentState) {
        case AppState::HOME:
            // Renders camera chassis, video feed, tally lamp, and 4 cards
            homeView.draw(frameBuffer, currentFrame);
            break;

        case AppState::QUAD_VIEW:
            // Renders 2x2 comparison matrix
            quadView.draw(frameBuffer, currentFrame);
            break;

        case AppState::MODE_VIEW:
            // Renders focused single filter with intensity adjustment
            modeView.draw(frameBuffer, currentFrame);
            break;

        case AppState::FILTER_STUDIO:
            // Renders manual DSP filter controls
            studioView.draw(frameBuffer, currentFrame);
            break;
    }

    // The UI stays landscape while the media frame can rotate in 90-degree
    // increments for portrait footage.
    const cv::Scalar controlFill(24, 24, 30);
    const cv::Scalar controlText(230, 230, 230);
    cv::rectangle(frameBuffer, cv::Rect(1110, 18, 62, 38), controlFill, -1);
    cv::rectangle(frameBuffer, cv::Rect(1110, 18, 62, 38), cv::Scalar(110, 110, 120), 1);
    cv::putText(frameBuffer, "<", cv::Point(1130, 45), cv::FONT_HERSHEY_SIMPLEX, 0.8, controlText, 2);
    cv::rectangle(frameBuffer, cv::Rect(1180, 18, 62, 38), controlFill, -1);
    cv::rectangle(frameBuffer, cv::Rect(1180, 18, 62, 38), cv::Scalar(110, 110, 120), 1);
    cv::putText(frameBuffer, ">", cv::Point(1200, 45), cv::FONT_HERSHEY_SIMPLEX, 0.8, controlText, 2);
}

// ==============================================================================
// DRAW: Sends the finished OpenCV frameBuffer to the screen via openFrameworks
// ==============================================================================
void ofApp::draw() {
    // 1. Reset color tint to pure white so the image displays at full brightness
    ofSetColor(255, 255, 255, 255);

    if (!frameBuffer.empty()) {
        // 2. OpenCV stores pixels as Blue-Green-Red (BGR).
        // openFrameworks / OpenGL requires Red-Green-Blue (RGB).
        // We convert the colors here so the colors look natural and correct.
        cv::Mat displayMat;
        cv::cvtColor(frameBuffer, displayMat, cv::COLOR_BGR2RGB);

        // Keep the upload buffer contiguous for safe OpenFrameworks texture updates.
        cv::Mat continuousMat = displayMat.clone();

        // 3. Copy CPU pixels into openFrameworks ofImage
        displayImage.setFromPixels(continuousMat.data, continuousMat.cols, continuousMat.rows, OF_IMAGE_COLOR);

        // 4. CRITICAL: update() transfers the pixel data to the graphics card (GPU).
        // Without this line, the screen remains blank gray!
        displayImage.update();

        // Preserve the 16:9 canvas aspect ratio in resized or portrait windows.
        // The UI remains a landscape canvas with letterboxing rather than being
        // stretched or rotated into an unusable coordinate system.
        const float scale = std::min(static_cast<float>(ofGetWidth()) / kCanvasWidth,
                                     static_cast<float>(ofGetHeight()) / kCanvasHeight);
        const float drawWidth = kCanvasWidth * scale;
        const float drawHeight = kCanvasHeight * scale;
        const float offsetX = (ofGetWidth() - drawWidth) * 0.5f;
        const float offsetY = (ofGetHeight() - drawHeight) * 0.5f;

        ofSetColor(0, 0, 0, 255);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(255, 255, 255, 255);
        displayImage.draw(offsetX, offsetY, drawWidth, drawHeight);
    }
}

// ==============================================================================
// MOUSE PRESSED: Handles clicks for navigating between screens
// ==============================================================================
void ofApp::mousePressed(int x, int y, int button) {
    const float scale = std::min(static_cast<float>(ofGetWidth()) / kCanvasWidth,
                                 static_cast<float>(ofGetHeight()) / kCanvasHeight);
    const float offsetX = (ofGetWidth() - kCanvasWidth * scale) * 0.5f;
    const float offsetY = (ofGetHeight() - kCanvasHeight * scale) * 0.5f;
    if (scale <= 0.0f || x < offsetX || y < offsetY ||
        x >= offsetX + kCanvasWidth * scale || y >= offsetY + kCanvasHeight * scale) {
        return;
    }

    const int canvasX = static_cast<int>((x - offsetX) / scale);
    const int canvasY = static_cast<int>((y - offsetY) / scale);

    if (canvasY >= 18 && canvasY < 56 && canvasX >= 1110 && canvasX < 1172) {
        mediaManager.rotateLeft();
        return;
    }
    if (canvasY >= 18 && canvasY < 56 && canvasX >= 1180 && canvasX < 1242) {
        mediaManager.rotateRight();
        return;
    }

    if (currentState == AppState::HOME) {
        // Ask HomeView which card was clicked in the fixed application canvas.
        HomeAction action = homeView.handleMouseClicked(canvasX, canvasY);

        if (action == HomeAction::PLAY_VIEW) {
            currentState = AppState::QUAD_VIEW;
        } 
        else if (action == HomeAction::SELECT_MODE) {
            modeView.setFilter(&retroFilter, "1950s Retro Mode");
            currentState = AppState::MODE_VIEW;
        } 
        else if (action == HomeAction::UPLOAD_STREAM) {
            if (mediaManager.openFileDialog()) {
                currentState = AppState::FILTER_STUDIO;
            }
        } 
        else if (action == HomeAction::MANUAL_FILTER) {
            currentState = AppState::FILTER_STUDIO;
        }
    } 
    else if (currentState == AppState::QUAD_VIEW) {
        // Click on any of the 4 quadrants to zoom directly into that filter
        int quadIndex = quadView.handleMouseClicked(canvasX, canvasY);
        if (quadIndex == 1) { 
            modeView.setFilter(&retroFilter, "1950s Retro Mode"); 
            currentState = AppState::MODE_VIEW; 
        } else if (quadIndex == 2) { 
            modeView.setFilter(&holidayFilter, "Holiday Warmth Mode"); 
            currentState = AppState::MODE_VIEW; 
        } else if (quadIndex == 3) { 
            modeView.setFilter(&partyFilter, "Party Neon Mode"); 
            currentState = AppState::MODE_VIEW; 
        }
    }
}

// ==============================================================================
// KEY PRESSED: Handles keyboard shortcuts
// ==============================================================================
void ofApp::keyPressed(int key) {
    // Pressing ESC always brings the user back to the Home View
    if (key == OF_KEY_ESC) {
        currentState = AppState::HOME;
    }
    else if (key == '[') {
        mediaManager.rotateLeft();
    }
    else if (key == ']') {
        mediaManager.rotateRight();
    }
    // In Mode View: '+' increases intensity, '-' decreases intensity
    else if (currentState == AppState::MODE_VIEW) {
        if (key == '+' || key == '=') {
            modeView.setIntensity(modeView.getIntensity() + 0.05f);
        }
        if (key == '-' || key == '_') {
            modeView.setIntensity(modeView.getIntensity() - 0.05f);
        }
    }
    else if (currentState == AppState::FILTER_STUDIO) {
        auto& filter = studioView.getFilter();

        if (key == 'B') {
            manualBrightness = std::min(100.0f, manualBrightness + 5.0f);
            filter.setBrightness(manualBrightness);
        } else if (key == 'b') {
            manualBrightness = std::max(-100.0f, manualBrightness - 5.0f);
            filter.setBrightness(manualBrightness);
        } else if (key == 'C') {
            manualContrast = std::min(3.0f, manualContrast + 0.1f);
            filter.setContrast(manualContrast);
        } else if (key == 'c') {
            manualContrast = std::max(0.1f, manualContrast - 0.1f);
            filter.setContrast(manualContrast);
        } else if (key == 'S') {
            manualSharpness = std::min(5.0f, manualSharpness + 0.1f);
            filter.setSharpness(manualSharpness);
        } else if (key == 's') {
            manualSharpness = std::max(0.0f, manualSharpness - 0.1f);
            filter.setSharpness(manualSharpness);
        } else if (key == 'I' || key == 'i') {
            manualInvert = !manualInvert;
            filter.setInvert(manualInvert);
        } else if (key == 'R' || key == 'r') {
            manualBrightness = 0.0f;
            manualContrast = 1.0f;
            manualSharpness = 0.0f;
            manualInvert = false;
            filter.reset();
        }
    }
}
