cmake_minimum_required(VERSION 3.22)
project(gamepad_ui)

set(CMAKE_CXX_STANDARD 14)
set(Sources src/main.cpp)

add_executable(gamepad_ui ./src/main.cpp src/streamer.cpp include/drc/streamer.h include/drc/types.h include/drc/pixel-format.h include/drc/input.h include/drc/video-frame-rate.h)
