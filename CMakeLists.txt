cmake_minimum_required(VERSION 3.16)

project(TriviaGame VERSION 0.1.0)
configure_file(TriviaGameConfig.h.in TriviaGameConfig.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src/server/include)
file(GLOB SOURCES "src/server/*.cpp")

add_executable(TriviaGameServer ${SOURCES})

target_include_directories(TriviaGameServer PUBLIC "${PROJECT_BINARY_DIR}")
