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
		void update();
		cv::Mat getCurrentFrame();

	private:
		cv::Mat currentFrame;

		// Source objects
    ofVideoPlayer videoPlayer;
    ofVideoGrabber webcam;
    
	// Status to check which source is active
    enum SourceType { NONE, IMAGE, VIDEO, WEBCAM};
    SourceType activeSource = NONE;

	void generateSMPTPattern(); // helper method to generate SMPTE color bars
};

