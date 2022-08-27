cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(raster-pipeline VERSION 1.0)
add_executable(raster-pipeline src/geometry.cpp src/camera.cpp src/shapes.cpp src/color.cpp src/canvas.cpp src/projector.cpp src/main.cpp src/frame_buffer.cpp)
target_include_directories(raster-pipeline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)
