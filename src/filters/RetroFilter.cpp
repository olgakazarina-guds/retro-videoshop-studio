#include "RetroFilter.h"

cv::Mat RetroFilter::process(const cv::Mat& input, float intensity) {
    // Safety check: if input frame is empty, return it directly to avoid crash
    if (input.empty()) return input;

    // 1. Always convert to grayscale, so it never reverts to original
    cv::Mat gray, gray3c;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, gray3c, cv::COLOR_GRAY2BGR);

    // 2. Apply Denim/Indigo Vintage Tint (Boost Blue, slightly reduce Red/Green)
    cv::Mat denimMat;
    gray3c.convertTo(denimMat, CV_32F);

    std::vector<cv::Mat> channels;
    cv::split(denimMat, channels);
    
	channels[0] *= 1.05f; // Blue channel boosted for denim effect
    channels[1] *= 0.98f; // Green channel nearly neutral
    channels[2] *= 0.90f; // Red channel softened slightly
    
    cv::merge(channels, denimMat);
    denimMat.convertTo(denimMat, input.type());

	//Base output is the clean retro denim look
	cv::Mat filtered = denimMat.clone();

    // 3. Add VHS/Tape Breakup Lines (Horizontal glitch streaks)
    int lineCount = static_cast<int>(4 * intensity);
    
    for (int i = 0; i < lineCount; ++i) {
        int yPos = rand() % filtered.rows;
        int thickness = 1;
        
        cv::Scalar lineColor = (rand() % 2 == 0) ? cv::Scalar(50, 50, 50) : cv::Scalar(160, 160, 160);
        cv::line(filtered, cv::Point(0, yPos), cv::Point(filtered.cols, yPos), lineColor, thickness);
    }

    // 4. Weighted Frame Blending (Formula: Output = Source*(1-Intensity) + Filtered*Intensity)
    cv::Mat output;
    cv::addWeighted(gray3c, 1.0f - intensity, filtered, intensity, 0.0, output);

    return output;
}
