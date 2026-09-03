cmake_minimum_required(VERSION 3.10)
project(ASCII_camera)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# find OpenCV library in the system
find_package(OpenCV REQUIRED)

# compile main.cpp into an executable of a given name
add_executable(ascii_camera main.cpp)

# link executable with OpenCV library
target_link_libraries(ascii_camera ${OpenCV_LIBS})
