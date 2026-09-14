#include "AsciiApp.hpp"
#include <iostream>


int main() {
    std::cout << "Starting program..." << std::endl;

    try {
        AsciiApp app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}