cmake_minimum_required(VERSION 3.21)
project(GNCS)

set(CMAKE_CXX_STANDARD 14)
include_directories(Includes /usr/local/Cellar/jsoncpp/1.9.5/include)
LINK_DIRECTORIES(/usr/local/Cellar/jsoncpp/1.9.5/lib)

add_executable(GNCS main.cpp IO/base64.c Comm/Server.cpp Cmd/Command.cpp Cmd/cmdExec.cpp Includes/color_def.h)
target_link_libraries(GNCS libjsoncpp.25.dylib)