cmake_minimum_required(VERSION 3.16)

project(TriviaGame VERSION 0.1.0
                   DESCRIPTION "Multiplayer Trivia Game"
                   LANGUAGES CXX)

configure_file(TriviaGameConfig.h.in TriviaGameConfig.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src/server/include)
file(GLOB SERVER_SOURCES "src/server/*.cpp")
file(GLOB CLIENT_SOURCES "src/client/*.cpp")

add_executable(TriviaGameServer ${SERVER_SOURCES})
add_executable(TriviaGameClient ${CLIENT_SOURCES})

target_include_directories(TriviaGameServer PUBLIC "${PROJECT_BINARY_DIR}")

file(GLOB TEST_SOURCES "tests/*.test.cpp")
add_executable(TriviaTests
  tests/TestMain.cpp
  ${TEST_SOURCES})
 
target_include_directories(TriviaTests
  PUBLIC externals/catch2/)
 
add_test(
  NAME TriviaTests
  COMMAND TriviaTests -o test_report.xml -r junit)

enable_testing()

