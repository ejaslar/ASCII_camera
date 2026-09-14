#pragma once
#include <opencv2/opencv.hpp>
#include "AsciiRenderer.hpp"


// class encapsulating app logic
class AsciiApp {
private:
    cv::VideoCapture cap;
    AsciiRenderer renderer;
    static AsciiApp* instance;

    void cleanup();
    static void signalHandler(int sig);

public:
    AsciiApp();
    ~AsciiApp();
    void run();
};