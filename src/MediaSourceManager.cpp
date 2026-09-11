#include "MediaSourceManager.h"

MediaSourceManager::MediaSourceManager() {
        currentFrame = cv::Mat::zeros(720, 1280, CV_8UC3);
		generateSMPTPattern();
		}

bool MediaSourceManager::openFileDialog() {
    // Ask the operating system to let the user choose a media file.
    ofFileDialogResult result = ofSystemLoadDialog("Select an image or video source");
    
    if (result.bSuccess) {
        std::string filePath = result.getPath();
        std::string ext = ofToLower(ofFilePath::getFileExt(filePath));
        
        // The file extension tells us which loader should handle the source.
        if (ext == "jpg" || ext == "jpeg" || ext == "png") {
            return loadImage(filePath);
        } 
        else if (ext == "mp4" || ext == "mov" || ext == "avi" || ext == "mkv") {
            return loadVideo(filePath);
        } 
        else {
            ofLogWarning("MediaSourceManager") << "Unsupported file format: " << ext;
        }
    }
	return false; // user canceled or invalid format
}

bool MediaSourceManager::loadImage(const std::string& path) {
	// Close any previously loaded video to free resources
	if (videoPlayer.isLoaded()) {
		videoPlayer.close();
	}
	webcam.close();

    ofImage img;
    // 1. Attempt to load the file from the bin/data folder
    if (img.load(path)) {
        ofPixels& pixels = img.getPixels();
        int srcType = (pixels.getImageType() == OF_IMAGE_COLOR_ALPHA) ? CV_8UC4 : CV_8UC3;
        
        // Wrap the OpenFrameworks pixel buffer in an OpenCV matrix.
        // This lets the existing OpenCV filters work on the selected image.
        cv::Mat temp(static_cast<int>(pixels.getHeight()), static_cast<int>(pixels.getWidth()), srcType, pixels.getData());
        
        // Convert to standard OpenCV BGR format
        if (srcType == CV_8UC4) {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGBA2BGR);
        } else {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
        }
        applyRotation();
		activeSource = IMAGE;
		return true;
	}

    else {
        // 2. Fallback: If Test.jpg is not found in bin/data, generate a classic SMPTE broadcast test pattern
        // This ensures the application never starts with a blank gray window!
        ofLogNotice("MediaSourceManager") << "File not found: " << path << ". Generating vintage color-bar test pattern.";
		generateSMPTPattern();
		activeSource = NONE;
		return false;
	}
}

bool MediaSourceManager::loadVideo(const std::string& path) {
    webcam.close();
    if (videoPlayer.isLoaded()) {
        videoPlayer.close();
    }
    if (videoPlayer.load(path)) {
        videoPlayer.setLoopState(OF_LOOP_NORMAL);
        videoPlayer.play();
        activeSource = VIDEO;
        return true;
    }

    ofLogWarning("MediaSourceManager") << "Could not load video: " << path;
    generateSMPTPattern();
    activeSource = NONE;
    return false;
}

bool MediaSourceManager::openWebcam(int deviceID) {
    if (videoPlayer.isLoaded()) {
        videoPlayer.close();
    }
    webcam.close();
    webcam.setDeviceID(deviceID);
    if (webcam.setup(1280, 720)) {
        activeSource = WEBCAM;
        return true;
    }

    ofLogWarning("MediaSourceManager") << "Webcam unavailable; using test pattern.";
    generateSMPTPattern();
    activeSource = NONE;
    return false;
}

void MediaSourceManager::rotateLeft() {
    // Four quarter-turns bring the image back to its original orientation.
    rotationQuarterTurns = (rotationQuarterTurns + 3) % 4;
    applyRotation();
}

void MediaSourceManager::rotateRight() {
    rotationQuarterTurns = (rotationQuarterTurns + 1) % 4;
    applyRotation();
}

void MediaSourceManager::applyRotation() {
    if (currentFrame.empty() || rotationQuarterTurns == 0) {
        return;
    }

    cv::Mat rotated;
    // OpenCV creates a new matrix with the requested orientation.
    // We rotate only the media frame, not the application window or UI.
    switch (rotationQuarterTurns) {
        case 1:
            cv::rotate(currentFrame, rotated, cv::ROTATE_90_CLOCKWISE);
            break;
        case 2:
            cv::rotate(currentFrame, rotated, cv::ROTATE_180);
            break;
        case 3:
            cv::rotate(currentFrame, rotated, cv::ROTATE_90_COUNTERCLOCKWISE);
            break;
    }
    currentFrame = rotated;
}

void MediaSourceManager::update() {
    if (activeSource == VIDEO) {
        // Video players need to advance one frame at a time in update().
        videoPlayer.update();
        if (videoPlayer.isFrameNew()) {
            ofPixels& pixels = videoPlayer.getPixels();
            cv::Mat temp(static_cast<int>(pixels.getHeight()), static_cast<int>(pixels.getWidth()), CV_8UC3, reinterpret_cast<void*>(pixels.getData()));
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
            // Apply the user's selected orientation to every new video frame.
            applyRotation();
        }
    } else if (activeSource == WEBCAM) {
        webcam.update();
        if (webcam.isFrameNew()) {
            ofPixels& pixels = webcam.getPixels();
            cv::Mat temp(static_cast<int>(pixels.getHeight()), static_cast<int>(pixels.getWidth()), CV_8UC3, pixels.getData());
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
            applyRotation();
        }
    }
}

cv::Mat MediaSourceManager::getCurrentFrame() {
    return currentFrame;
}

// Standalone helper method to generate a classic SMPTE broadcast test pattern
void MediaSourceManager::generateSMPTPattern() {
		currentFrame = cv::Mat::zeros(720, 1280, CV_8UC3);
	
        // 7 Classic SMPTE Color Bars (BGR color format)
        cv::Scalar bars[7] = {
            cv::Scalar(200, 200, 200), // White
            cv::Scalar(0, 220, 220),   // Yellow
            cv::Scalar(220, 220, 0),   // Cyan
            cv::Scalar(0, 220, 0),     // Green
            cv::Scalar(220, 0, 220),   // Magenta
            cv::Scalar(0, 0, 220),     // Red
            cv::Scalar(220, 0, 0)      // Blue
        };
        
        int barW = 1280 / 7;
        for (int i = 0; i < 7; ++i) {
            cv::Rect r(i * barW, 0, (i == 6 ? 1280 - i * barW : barW), 720);
            cv::rectangle(currentFrame, r, bars[i], -1);
        }
        
        // Broadcast test label
        cv::putText(currentFrame, "RETRO VIDEOSHOP - SMPTE TEST PATTERN", cv::Point(320, 360), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(20, 20, 20), 3);
        cv::putText(currentFrame, "RETRO VIDEOSHOP - SMPTE TEST PATTERN", cv::Point(320, 360), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 255, 255), 2);

	}
