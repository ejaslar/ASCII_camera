#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // open MacBook's built-in camera - id 1 (id 0 is iPhone's continuity camera)
    std::cout << "Starting program..." << std::endl;
    std::cout << "Searching for camera..." << std::endl;

    cv::VideoCapture cap(1);

    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open the camera." << std::endl;
        return -1;
    }

    // prepare environment (assuming black terminal);
    // character palette
    const std::string ASCII_CHARS = " .:-=+*#%@";

    // target width of terminal window
    const int terminalWidth = 200;

    cv::Mat frame, grayFrame, resizedFrame;
    std::cout << "Press 'Ctrl + C' to exit." << std::endl;


    // clear terminal window by ANSI control code
    std::cout << "\x1b[2j";

    // loop retrieving frames from camera
    while (true) {
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error: Empty frame. Disconnecting..." << std::endl;
            break;
        }

        // make mirror reflection
        cv::flip(frame, frame, 1);

        // convert image to grayscale colorspace
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        // calculate new frame ratios (assuming font height to width ratio is 2)
        float frameRatio = (float)grayFrame.rows / grayFrame.cols;
        int terminalHeight = terminalWidth * frameRatio * 0.5f;

        // change frame size
        cv::resize(grayFrame, resizedFrame, cv::Size(terminalWidth, terminalHeight));

        // build frame from ascii characters;
        // ANSI code \x1b[H moves cursor to top left corner to override text without flickering
        std::string asciiFrame = "\x1b[H";

        // iterate over pixels of resized frame
        for (int y = 0; y < resizedFrame.rows; ++y) {
            for (int x = 0; x < resizedFrame.cols; ++x) {
                // retrieve pixel brightness (0 - black; 255 - white)
                int pixelBrightness = resizedFrame.at<uchar>(y, x);

                // convert pixel into ascii char;
                // pixelBrightness / 255 is a number form interval [0, 1];
                // we multiply it by ascii_chars length to choose char
                // based on this ratio from our char pallete
                int index = pixelBrightness * (ASCII_CHARS.length() - 1) / 255;
                asciiFrame += ASCII_CHARS[index];
            }
            // the end of the row - we go to the next one
            asciiFrame += "\n";
        }
        std::cout << asciiFrame << std::flush;
    }

    // release resources
    cap.release();
    
    return 0;
}