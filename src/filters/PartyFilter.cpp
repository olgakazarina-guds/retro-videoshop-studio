#include "PartyFilter.h"

cv::Mat PartyFilter::process(const cv::Mat& input, float intensity) {
    // Safety check: if input frame is empty, return it directly to avoid crash
    if (input.empty()) return input;

    cv::Mat processed;

    // STEP 1: Increase contrast and brightness for a vibrant neon look.
    input.convertTo(processed, -1, 1.35, 10);

    // STEP 2: Color Inversion using OpenCV's bitwise_not.
    cv::Mat inverted;
    cv::bitwise_not(processed, inverted);

    // STEP 3: Matrix Channel Manipulation (cv::split & cv::merge).
    std::vector<cv::Mat> channels;
    cv::split(processed, channels); // channels[0] = Blue, channels[1] = Green, channels[2] = Red

    // Boost the Blue and Red channels to create a futuristic synthwave/cyberpunk tint
    channels[0] = channels[0] * 1.25; // 25% boost to Blue
    channels[2] = channels[2] * 1.30; // 30% boost to Red

    // Recombine the modified channels back into a single 3-channel image matrix
    cv::merge(channels, processed);

    // Blend inverted highlights with the neon processed frame (75% neon + 25% inverted)
    cv::addWeighted(processed, 0.75, inverted, 0.25, 0.0, processed);

    // STEP 4: Weighted Frame Blending (Formula: Output = Source*(1-Intensity) + Filtered*Intensity)
    cv::Mat output;
    cv::addWeighted(input, 1.0f - intensity, processed, intensity, 0.0, output);

    return output;
}
