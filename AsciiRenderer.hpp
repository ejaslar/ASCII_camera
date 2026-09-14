#pragma once
#include <string>
#include <opencv2/opencv.hpp>


// class that renders ascii frame from a matrix captured from camera (cv::Mat)
class AsciiRenderer {
private:
    std::string ASCII_CHARS;
    int targetWidth;
    cv::Mat resizedColor;
    cv::Mat resizedGray;

public:
    AsciiRenderer(int width = 120);
    std::string renderFrame(const cv::Mat& frame);
};