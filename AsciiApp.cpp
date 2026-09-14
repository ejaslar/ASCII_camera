#include "AsciiApp.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <termios.h>
#include <unistd.h>


AsciiApp* AsciiApp::instance = nullptr;

// c-tor: open camera and prepare terminal;
// id 1 is MacBook's built-in camera (id 0 is iPhone's continuity camera)
AsciiApp::AsciiApp() : cap(1), renderer(120), isRunning(true) {
    std::cout << "Searching for camera..." << std::endl;
    if (!cap.isOpened()) {
        throw std::runtime_error("Cannot open the camera.");
    }

    instance = this;

    // clear terminal window by ANSI control code and hide the cursor
    std::cout << "\x1b[2J\x1b[?25l";

    // disable printing ^C (cuz it destroys a frame after ctrl+C)
    struct termios term;
    tcgetattr(STDIN_FILENO, &term); // retrieve current terminal settings
    term.c_lflag &= ~ECHOCTL;       // delete flag ECHOCTL (Echo Control Characters)
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // apply changes immediately

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
    while (isRunning) {
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

    // restore ^C printing
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHOCTL; // add flag ECHOCTL back
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void AsciiApp::signalHandler(int sig) {
    if (instance) {
        instance->isRunning = false;
    }
}