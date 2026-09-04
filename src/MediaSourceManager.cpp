#include "MediaSourceManager.h"
void MediaSourceManager::loadImage(const std::string& path) {
    // Video- oder Webcam-Quellen stoppen, falls aktiv
    //if (videoPlayer.isLoaded()) videoPlayer.close();
    // if (webcam.isInitialized()) webcam.close();

	ofImage img;
	if (img.load(path)) {
        ofPixels& pixels = img.getPixels();
        int srcType = (pixels.getImageType() == OF_IMAGE_COLOR_ALPHA) ? CV_8UC4 : CV_8UC3;
        
        cv::Mat temp(static_cast<int>(pixels.getHeight()), static_cast<int>(pixels.getWidth()), srcType, pixels.getData());
        
		// Keep currentFrame in standard RGB for openFrameworks rendering
        if (srcType == CV_8UC4) {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGBA2BGR);
        } else {
            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
        }
    } else {
        ofLogNotice("MediaSourceManager") << "Couldn't load image: " << path;
    }
}

//void MediaSourceManager::loadVideo(const std::string& path) {
//    if (webcam.isInitialized()) webcam.close();
//
//    if (videoPlayer.load(path)) {
//        videoPlayer.play();
//        activeSource = VIDEO;
//    } else {
//        ofLogNotice("MediaSourceManager") << "Konnte Video nicht laden: " << path;
//    }
//}

// void MediaSourceManager::openWebcam(int deviceID) {
   // if (videoPlayer.isLoaded()) videoPlayer.close();

   // webcam.setDeviceID(deviceID);
   // if (webcam.setup(640, 480)) {
   //     activeSource = WEBCAM;
   // } else {
   //     ofLogNotice("MediaSourceManager") << "Konnte Webcam nicht öffnen (ID: " << deviceID << ")";
  //  }
//}

//void MediaSourceManager::update() {
//    if (activeSource == VIDEO) {
//        videoPlayer.update();
//        if (videoPlayer.isFrameNew()) {
//            ofPixels& pixels = videoPlayer.getPixels();
//            cv::Mat temp(pixels.getHeight(), pixels.getWidth(), CV_8UC3, pixels.getData());
//            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
//        }
//    } 
//    else if (activeSource == WEBCAM) {
//        webcam.update();
//        if (webcam.isFrameNew()) {
//            ofPixels& pixels = webcam.getPixels();
//            cv::Mat temp(pixels.getHeight(), pixels.getWidth(), CV_8UC3, pixels.getData());
//            cv::cvtColor(temp, currentFrame, cv::COLOR_RGB2BGR);
//        }
//    }
//    // Bei IMAGE muss pro Frame nichts aktualisiert werden, da das Bild statisch ist.
//}

cv::Mat MediaSourceManager::getCurrentFrame() {
    return currentFrame;
}
