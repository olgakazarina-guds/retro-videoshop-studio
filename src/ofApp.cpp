#include "ofApp.h"

void ofApp::setup() {
    ofSetWindowTitle("Retro Videoshop Studio");
    ofSetWindowShape(1280, 720);
	mediaManager.loadImage("Test.jpg"); // Load a default image for testing
    currentState = AppState::HOME;
    frameBuffer = cv::Mat::zeros(720, 1280, CV_8UC3);
frameBuffer = cv::Mat::zeros(720, 1280, CV_8UC3);

    // Disable default behavior of ESC key quitting the app, allowing us to use it for navigation
    ofSetEscapeQuitsApp(false);

    // Initialize media feed (defaults to webcam or demo reel)
    //mediaManager.openWebcam();
}

void ofApp::update() {
	  // 1. Advance media frame
	  // mediaManager.update();
	  cv::Mat currentFrame = mediaManager.getCurrentFrame();

     // 2. Delegate rendering to the active view based on state
		switch (currentState) {
			case AppState::HOME:
		           homeView.draw(frameBuffer, currentFrame);
            break;
        case AppState::QUAD_VIEW:
            quadView.draw(frameBuffer, currentFrame);
            break;
        case AppState::MODE_VIEW:
            modeView.draw(frameBuffer, currentFrame);
            break;
        case AppState::FILTER_STUDIO:
            studioView.draw(frameBuffer, currentFrame);
            break;
    }
}

void ofApp::draw() {
    if (!frameBuffer.empty()) {
		cv::Mat displayMat;
		cv::cvtColor(frameBuffer, displayMat, cv::COLOR_BGR2RGB); // Convert BGR to RGB for correct color display
        ofImage img;
        img.setFromPixels(displayMat.data, displayMat.cols, displayMat.rows, OF_IMAGE_COLOR);
        img.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
}

void ofApp::mousePressed(int x, int y, int button) {
    if (currentState == AppState::HOME) {
        HomeAction action = homeView.handleMouseClicked(x, y);
        if (action == HomeAction::PLAY_VIEW) {
            currentState = AppState::QUAD_VIEW;
        } else if (action == HomeAction::SELECT_MODE) {
            modeView.setFilter(&retroFilter, "1950s Retro Mode");
            currentState = AppState::MODE_VIEW;
		}
		else if (action == HomeAction::UPLOAD_STREAM) {
			// For simplicity, we will just load a default image for now
			mediaManager.loadImage("Test.jpg");
			currentState = AppState::FILTER_STUDIO;
        } else if (action == HomeAction::MANUAL_FILTER) {
            currentState = AppState::FILTER_STUDIO;
        }
    } 
    else if (currentState == AppState::QUAD_VIEW) {
        // Click-to-mode routing from 2x2 matrix
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

void ofApp::keyPressed(int key) {
    // ESC key always returns to the main Home dashboard
    if (key == OF_KEY_ESC) {
        currentState = AppState::HOME;
    }
    // Intensity adjustments in ModeView
    else if (currentState == AppState::MODE_VIEW) {
        if (key == '+' || key == '=') modeView.setIntensity(modeView.getIntensity() + 0.05f);
        if (key == '-' || key == '_') modeView.setIntensity(modeView.getIntensity() - 0.05f);
    }
}
