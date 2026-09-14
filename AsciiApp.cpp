#include "AsciiApp.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>


AsciiApp* AsciiApp::instance = nullptr;

// c-tor: open camera and prepare terminal;
// id 1 is MacBook's built-in camera (id 0 is iPhone's continuity camera)
AsciiApp::AsciiApp() : cap(1), renderer(120) {
    std::cout << "Searching for camera..." << std::endl;
    if (!cap.isOpened()) {
        throw std::runtime_error("Cannot open the camera.");
    }

    instance = this;

    // clear terminal window by ANSI control code and hide the cursor
    std::cout << "\x1b[2J\x1b[?25l";

    // handle SIGINT;
    // it'll work without AsciiApp:: namespace, but it indicates that
    // signalHandler is a class function and not some global one from the file;
    signal(SIGINT, AsciiApp::signalHandler);
}

// d-tor: cleanup
AsciiApp::~AsciiApp() {
    cleanup();
}

// run main loop
void AsciiApp::run() {
    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            throw std::runtime_error("Problem with camera. Captured frame is empty.");
        }

        // make mirror reflection of the frame
        cv::flip(frame, frame, 1);

        // render frame in terminal window
        std::cout << renderer.renderFrame(frame) << std::flush;
    }
}

void AsciiApp::cleanup() {
    // bring back the cursor and reset text color
    std::cout << "\x1b[?25h\x1b[0m";
    std::cout << "\nClosing ASCII camera...\n";
    if (cap.isOpened()) {
        cap.release();
    }
}

void AsciiApp::signalHandler(int sig) {
    if (instance) {
        instance->cleanup();
        exit(0);
    }
}