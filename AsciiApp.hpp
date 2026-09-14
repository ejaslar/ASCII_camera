#pragma once
#include <opencv2/opencv.hpp>
#include "AsciiRenderer.hpp"
#include <atomic>


// class encapsulating app logic
class AsciiApp {
private:
    cv::VideoCapture cap;
    AsciiRenderer renderer;
    static AsciiApp* instance;
    std::atomic<bool> isRunning;

    void cleanup();
    static void signalHandler(int sig);

public:
    AsciiApp();
    ~AsciiApp();
    void run();
};