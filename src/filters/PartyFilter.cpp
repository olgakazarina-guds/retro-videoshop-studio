#include "PartyFilter.h"
#include <algorithm>

cv::Mat PartyFilter::process(const cv::Mat& input, float intensity) {
    // Safety check: if input frame is empty, return it directly to avoid crash
    if (input.empty()) return input;

    intensity = std::max(0.0f, std::min(1.0f, intensity));

    cv::Mat processed;

    // STEP 1: Increase contrast and brightness for a vibrant neon look.
    input.convertTo(processed, -1, 1.35, 10);

    // STEP 2: Split-tone the image: cool blue shadows and warm magenta highlights.
    cv::Mat luminance;
    cv::cvtColor(processed, luminance, cv::COLOR_BGR2GRAY);
    cv::Mat shadowsMask, highlightsMask;
    cv::threshold(luminance, shadowsMask, 105, 255, cv::THRESH_BINARY_INV);
    cv::threshold(luminance, highlightsMask, 165, 255, cv::THRESH_BINARY);

    cv::Mat shadowTone(processed.size(), processed.type(), cv::Scalar(150, 35, 95));
    cv::Mat highlightTone(processed.size(), processed.type(), cv::Scalar(55, 35, 175));
    cv::Mat splitTone = processed.clone();
    shadowTone.copyTo(splitTone, shadowsMask);
    highlightTone.copyTo(splitTone, highlightsMask);
    cv::addWeighted(processed, 0.45, splitTone, 0.55, 0.0, processed);

    // STEP 3: Add channel boosts to reinforce the synthwave palette.
    std::vector<cv::Mat> channels;
    cv::split(processed, channels); // channels[0] = Blue, channels[1] = Green, channels[2] = Red

    // Boost the Blue and Red channels to create a futuristic synthwave/cyberpunk tint
    channels[0] = channels[0] * 1.25; // 25% boost to Blue
    channels[2] = channels[2] * 1.30; // 30% boost to Red

    // Recombine the modified channels back into a single 3-channel image matrix
    cv::merge(channels, processed);

    // STEP 4: Use inverted pixels only along strong edges for a highlighted neon outline.
    cv::Mat edges;
    cv::Canny(luminance, edges, 80, 160);
    cv::dilate(edges, edges, cv::Mat(), cv::Point(-1, -1), 1);

    cv::Mat inverted;
    cv::bitwise_not(processed, inverted);
    inverted.copyTo(processed, edges);

    // STEP 5: Weighted Frame Blending (Formula: Output = Source*(1-Intensity) + Filtered*Intensity)
    cv::Mat output;
    cv::addWeighted(input, 1.0f - intensity, processed, intensity, 0.0, output);

    return output;
}
