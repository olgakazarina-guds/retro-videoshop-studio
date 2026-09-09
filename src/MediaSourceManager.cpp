#include "MediaSourceManager.h"

MediaSourceManager::MediaSourceManager() {
        currentFrame = cv::Mat::zeros(720, 1280, CV_8UC3);
		generateSMPTPattern();
		}

bool MediaSourceManager::openFileDialog() {
    ofFileDialogResult result = ofSystemLoadDialog("Select an image or video source");
    
    if (result.bSuccess) {
        std::string filePath = result.getPath();
        std::string ext = ofToLower(ofFilePath::getFileExt(filePath));
        
        if (ext == "jpg" || ext == "jpeg" || ext == "png") {
            loadImage(filePath);
			return true;
        } 
        else if (ext == "mp4" || ext == "mov" || ext == "avi" || ext == "mkv") {
            loadVideo(filePath);
			return true;
        } 
        else {
            ofLogWarning("MediaSourceManager") << "Unsupported file format: " << ext;
        }
    }
	return false; // user canceled or invalid format
}

void MediaSourceManager::loadImage(const std::string& path) {
	// Close any previously loaded video to free resources
	if (videoPlayer.isLoaded()) {
		videoPlayer.close();
}

    ofImage img;
    // 1. Attempt to load the file from the bin/data folder
    if (img.load(path)) {
        ofPixels& pixels = img.getPixels();
        int srcType = (pixels.getImageType() == OF_IMAGE_COLOR_ALPHA) ? CV_8UC4 : CV_8UC3;
        
        // Wrap raw image data into an OpenCV cv::Mat
        cv::Mat temp(static_cast<int>(pixels.getHeight()), static_cast<int>(pixels.getWidth()), srcType, pixels.getData());
        
        // Convert to standard OpenCV BGR format
        if (srcType == CV_8UC4) {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGBA2BGR);
        } else {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
        }
		activeSource = IMAGE;
	}

    else {
        // 2. Fallback: If Test.jpg is not found in bin/data, generate a classic SMPTE broadcast test pattern
        // This ensures the application never starts with a blank gray window!
        ofLogNotice("MediaSourceManager") << "File not found: " << path << ". Generating vintage color-bar test pattern.";
		generateSMPTPattern();
		activeSource = NONE;
	}
}

void MediaSourceManager::loadVideo(const std::string& path) {
    if (videoPlayer.load(path)) {
        videoPlayer.play();
        activeSource = VIDEO;
    } else {
        ofLogNotice("MediaSourceManager") << "Konnte Video nicht laden: " << path;
    }
}

void MediaSourceManager::update() {
    if (activeSource == VIDEO) {
        videoPlayer.update();
        if (videoPlayer.isFrameNew()) {
            ofPixels& pixels = videoPlayer.getPixels();
            cv::Mat temp(static_cast<int>(pixels.getHeight()), static_cast<int>(pixels.getWidth()), CV_8UC3, reinterpret_cast<void*>(pixels.getData()));
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
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
