cmake_minimum_required(VERSION 3.21)
project(GNCS)

set(CMAKE_CXX_STANDARD 14)
include_directories(Includes)
add_executable(GNCS main.cpp Comn/bt.cpp Includes/bt.h IO/base64.c Includes/base64.h)
