#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class MediaSourceManager
{
	public:
		void loadImage(const std::string& path);
		void loadVideo(const std::string& path);
		void openWebcam(int deviceID = 0);
		void update();
		cv::Mat getCurrentFrame();

	private:
		cv::Mat currentFrame;
		//specific variables for video player or grabber

};

