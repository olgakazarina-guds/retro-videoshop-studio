#include "HolidayFilter.h"
cv::Mat HolidayFilter::process(const cv::Mat& input, float intensity) {
    if (input.empty()) return input;

    // Convert to grayscale base for a clean tonal foundation
    cv::Mat gray, gray3c;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, gray3c, cv::COLOR_GRAY2BGR);

    // 1. Build the Icy Blue state (active at intensity 0%)
    cv::Mat coldMat;
    gray3c.convertTo(coldMat, CV_32F);
    {
        std::vector<cv::Mat> channels;
        cv::split(coldMat, channels);
        channels[0] *= 1.35f; // Strong Blue channel boost
		channels[1] *= 1.10f; // Slight green lift for crisp icy feel
        channels[2] *= 0.65f; // heavily suppressed red
        cv::merge(channels, coldMat);
        coldMat.convertTo(coldMat, input.type());
    }

    // 2. Build the Warm Gold/Red state (active at intensity 100%)
    cv::Mat warmMat;
    gray3c.convertTo(warmMat, CV_32F);
    {
        std::vector<cv::Mat> channels;
        cv::split(warmMat, channels);
        channels[0] *= 0.55f; // Muted blue
        channels[1] *= 0.90f; // Balanced green
        channels[2] *= 1.45f; // Boosted red/gold warmth
        cv::merge(channels, warmMat);
        warmMat.convertTo(warmMat, input.type());
    }

    // 3. Scale smoothly between cold blue (0.0) and warm gold/red (1.0) via intensity
    cv::Mat output;
    cv::addWeighted(coldMat, 1.0f - intensity, warmMat, intensity, 0.0, output);

    return output;
}
