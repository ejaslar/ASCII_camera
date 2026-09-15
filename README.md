# ASCII camera

A C++ application that renders live webcam video in the terminal using ASCII characters.

# Key features
* Captures frames from the camera using the OpenCV library.
* Each ASCII character is chosen based on the pixel brightness and colored based on the original pixel values, using ANSI codes.
* Instead of using resource-intensive `system("clear")` command, which causes screen flickering, the program overwrites the frame previously rendered in terminal.
* Application shutdown: Uses `<termios.h>` to eliminate screen tearing and keyboard echo, and to prevent the OS from clearing I/O buffers when the application terminates.

# Tech stack
* C++17
* OpenCV5
* CMake

# How to build and run

The project uses the `CMake` build system. You must have the OpenCV library installed on your machine (e.g., via `brew install opencv` for macOS).

```bash
git clone https://github.com/ejaslar/ASCII_camera.git
cd ASCII_camera
mkdir build && cd build
cmake ..
cmake --build .
./ASCII_camera
```

# Compatibility and known limitations
The project was build and tested on MacBook M1 with macOS 27.

* **Continuity Camera vs Built-in Webcam:** On recent macOS versions, the OpenCV camera index `0` often defaults to a nearby iPhone (Continuity Camera). To bypass this behaviour, the code uses index `1` (`cv::VideoCapture(1)`), which corresponds to the MacBook's built-in camera.
    * If app does not work, it may be necessary to change this value back to `0` or different value, depending on whether the camera is built-in or not.

* **Operating System Support:** Due to its deep integration with with the UNIX API (`termios` library, POSIX signal structures), compiling and running this project on Windows will require modifications.
