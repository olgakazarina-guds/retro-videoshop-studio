#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class MediaSourceManager
{
	public:
		MediaSourceManager (); // Declare constructor
		bool openFileDialog();
		void loadImage(const std::string& path);
		void loadVideo(const std::string& path);
		//void openWebcam(int deviceID = 0);
		void update();
		cv::Mat getCurrentFrame();

	private:
		cv::Mat currentFrame;

		// Source objects
    ofVideoPlayer videoPlayer;
    
	// Status to check which source is active
    enum SourceType { NONE, IMAGE, VIDEO};
    SourceType activeSource = NONE;

	void generateSMPTPattern(); // helper method to generate SMPTE color bars
};

