#include "ofApp.h"

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

    // 4. Create an empty black 1280x720 canvas in OpenCV (8-bit, 3 color channels: BGR)
    frameBuffer = cv::Mat::zeros(720, 1280, CV_8UC3);

    // 5. Pre-allocate openFrameworks GPU texture memory to prevent lag during drawing
    displayImage.allocate(1280, 720, OF_IMAGE_COLOR);

    // 6. Load default test media (falls back to a vintage color-bar pattern if missing)
    // mediaManager.loadImage("Test.jpg");
}

// ==============================================================================
// UPDATE: Called continuously to calculate logic and process video frames
// ==============================================================================
void ofApp::update() {
	ofScopedLock lock(mutex); // Ensure thread-safe access to frameBuffer

	// 1. Update the media manager to fetch the latest video frame if a video is playing
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
}

// ==============================================================================
// DRAW: Sends the finished OpenCV frameBuffer to the screen via openFrameworks
// ==============================================================================
void ofApp::draw() {
	ofScopedLock lock(mutex); // Ensure thread-safe access to frameBuffer
    // 1. Reset color tint to pure white so the image displays at full brightness
    ofSetColor(255, 255, 255, 255);

    if (!frameBuffer.empty()) {
        // 2. OpenCV stores pixels as Blue-Green-Red (BGR).
        // openFrameworks / OpenGL requires Red-Green-Blue (RGB).
        // We convert the colors here so the colors look natural and correct.
        cv::Mat displayMat;
        cv::cvtColor(frameBuffer, displayMat, cv::COLOR_BGR2RGB);

		// Force a continuous clone to strip any Open CV row-padding bytes
		// to prevent a crash when copying to openFrameworks ofImage
		cv::Mat continuousMat = displayMat.clone();

		// Safely allocate or match displayImage dimensions if they ever change
		if (!displayImage.isAllocated() || displayImage.getWidth() != continuousMat.cols || displayImage.getHeight() != continuousMat.rows) {
			displayImage.allocate(continuousMat.cols, continuousMat.rows, OF_IMAGE_COLOR);
		}

        // 3. Copy CPU pixels into openFrameworks ofImage
        displayImage.setFromPixels(continuousMat.data, continuousMat.cols, continuousMat.rows, OF_IMAGE_COLOR);

        // 4. CRITICAL: update() transfers the pixel data to the graphics card (GPU).
        // Without this line, the screen remains blank gray!
        displayImage.update();

        // 5. Render the texture across the entire window
        displayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
}

// ==============================================================================
// MOUSE PRESSED: Handles clicks for navigating between screens
// ==============================================================================
void ofApp::mousePressed(int x, int y, int button) {
    if (currentState == AppState::HOME) {
        // Ask HomeView which card was clicked based on (x, y) coordinates
        HomeAction action = homeView.handleMouseClicked(x, y);

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
        int quadIndex = quadView.handleMouseClicked(x, y);
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
    // In Mode View: '+' increases intensity, '-' decreases intensity
    else if (currentState == AppState::MODE_VIEW) {
        if (key == '+' || key == '=') {
            modeView.setIntensity(modeView.getIntensity() + 0.05f);
        }
        if (key == '-' || key == '_') {
            modeView.setIntensity(modeView.getIntensity() - 0.05f);
        }
    }
}
