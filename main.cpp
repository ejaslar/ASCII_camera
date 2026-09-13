#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <csignal>
#include <cstdlib>
#include <exception>

// class that renders ascii frame from a matrix captured from camera (cv::Mat)
class AsciiRenderer {
private:
    std::string ASCII_CHARS;
    int targetWidth;
    cv::Mat resizedColor;
    cv::Mat resizedGray;

public:
    AsciiRenderer(int width = 120) : targetWidth(width), ASCII_CHARS(" .:-=+*#%@") {}

    std::string renderFrame(cv::Mat& frame) {
        // make mirror reflection of the frame
        cv::flip(frame, frame, 1);

        // calculate new frame ratios (assuming font height to width ratio is 2)
        float frameRatio = (float)frame.rows / frame.cols;
        int targetHeight = targetWidth * frameRatio * 0.5f;

        // change frame size
        cv::resize(frame, resizedColor, cv::Size(targetWidth, targetHeight));

        // convert resized frame to grayscale colorspace
        cv::cvtColor(resizedColor, resizedGray, cv::COLOR_BGR2GRAY);

        // build frame from ascii characters;
        // ANSI code \x1b[H moves cursor to top left corner to override text without flickering
        std::string asciiFrame = "\x1b[H";

        // optimisation: reserve string memory in advance to prevent string resizing in runtime;
        // 30 is an upper bound of one pixel memory (with safety buffer)
        asciiFrame.reserve(resizedColor.cols * resizedColor.rows * 30);

        // iterate over pixels of resizedColor frame
        for (int y = 0; y < resizedColor.rows; ++y) {
            for (int x = 0; x < resizedColor.cols; ++x) {
                // retrieve pixel bgr values
                cv::Vec3b color = resizedColor.at<cv::Vec3b>(y, x);
                int b = color[0];
                int g = color[1];
                int r = color[2];

                // retrieve pixel brightness (0 - black; 255 - white)
                int pixelBrightness = resizedGray.at<uchar>(y, x);

                // convert pixel into ascii char;
                // pixelBrightness / 255 is a number form interval [0, 1];
                // we multiply it by ascii_chars length to choose char
                // based on this ratio from our char pallete
                int index = pixelBrightness * (ASCII_CHARS.length() - 1) / 255;

                // build color sequence for given ascii char and add this char to the frame
                asciiFrame += "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
                asciiFrame += ASCII_CHARS[index];
            }
            // the end of the row - we go to the next one
            asciiFrame += "\n";
        }
        // reset terminal colors
        asciiFrame += "\x1b[0m";

        return asciiFrame;
    }
};


class AsciiApp {
private:
    cv::VideoCapture cap;
    AsciiRenderer renderer;

public:
    // c-tor: open camera and prepare terminal
    AsciiApp() : cap(1), renderer(120) {
        if (!cap.isOpened()) {
            throw std::runtime_error("Cannot open the camera.");
        }
        // clear terminal window by ANSI control code and hide the cursor
        std::cout << "\x1b[2J\x1b[?25l";
    }

    // d-tor: cleanup
    ~AsciiApp() {
        // bring back the cursor and reset text color
        std::cout << "\x1b[?25h\x1b[0m";
        if (cap.isOpened()) {
            cap.release();
        }
    }

    // run main loop
    void run() {
        cv::Mat frame;
        while (true) {
            cap >> frame;
            if (frame.empty()) break;
            std::cout << renderer.renderFrame(frame) << std::flush;
        }
    }
};


void handle_sigint(int sig) {
    // restore cursor
    std::cout << "\x1b[?25h";

    // reset text color
    std::cout << "\x1b[0m";

    // clear window
    std::cout << "\nClosing ASCII camera...\n";

    // terminate program
    exit(0);
}


int main() {
    // open MacBook's built-in camera - id 1 (id 0 is iPhone's continuity camera)
    std::cout << "Starting program..." << std::endl;
    std::cout << "Searching for camera..." << std::endl;

    // handle SIGINT
    signal(SIGINT, handle_sigint);

    try {
        AsciiApp app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}