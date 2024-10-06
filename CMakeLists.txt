cmake_minimum_required(VERSION 3.29)
project(ImageConvolution)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(ImageConvolution main.cpp
        kernels.cpp
        kernels.h)
target_link_libraries(ImageConvolution ${OpenCV_LIBS})
