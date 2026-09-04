#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // open MacBook's built-in camera - id 1 (id 0 is iPhone's continuity camera)
    std::cout << "Starting program and searching for camera..." << std::endl;
    cv::VideoCapture cap(1);

    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open the camera." << std::endl;
        return -1;
    }

    cv::Mat frame, grayFrame;
    std::cout << "Press 'q' to exit preview." << std::endl;

    // loop retrieving frames from camera
    while (true) {
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error: Empty frame. Disconnecting..." << std::endl;
            break;
        }

        // convert image to grayscale colorspace
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        // open image in a new window ("camera test" is the name of the window)
        cv::imshow("camera test", grayFrame);

        // wait 1 ms and after this execute remaining code (it'll go to another iteration - another image)
        // value 0 would mean "wait indefinitely" so the frame would be shown until user presses 'q'
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // release resources
    cap.release();
    cv::destroyAllWindows();

    return 0;
}