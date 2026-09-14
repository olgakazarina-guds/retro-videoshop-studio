#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class MediaSourceManager
{
	public:
		MediaSourceManager (); // Declare constructor
		bool openFileDialog();
		bool loadImage(const std::string& path);
		bool loadVideo(const std::string& path);
		bool openWebcam(int deviceID = 0);
		void rotateLeft();
		void rotateRight();
		void update();
		cv::Mat getCurrentFrame();

	private:
		ofMutex mutex; // Mutex for thread safety
		cv::Mat currentFrame;

		// Source objects
    ofVideoPlayer videoPlayer;
	ofVideoGrabber webcam;	// used for macOS / Xcode implementation
	cv::VideoCapture webcamCapture; // used for Windows implementation
    
	// Status to check which source is active
    enum SourceType { NONE, IMAGE, VIDEO, WEBCAM};
    SourceType activeSource = NONE;
    int rotationQuarterTurns = 0;

	void loadPlaceholder(); // show the bundled image when no usable media is available
	void generateSMPTPattern(); // final safety fallback if the placeholder is unavailable
	void applyRotation();
};
