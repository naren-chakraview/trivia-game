cmake_minimum_required(VERSION 3.10)

project(TriviaGame VERSION 0.1.0
                   DESCRIPTION "Multiplayer Trivia Game"
                   LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# bring in boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")
find_package(Threads)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(src/include)
file(GLOB SERVER_SOURCES "src/server/*.cpp")
file(GLOB CLIENT_SOURCES "src/client/*.cpp")

add_executable(TriviaGameServer ${SERVER_SOURCES})

target_link_libraries(TriviaGameServer PUBLIC ${Boost_SYSTEM_LIBRARY})
target_link_libraries (TriviaGameServer ${CMAKE_THREAD_LIBS_INIT})

add_executable(TriviaGameClient ${CLIENT_SOURCES})
target_link_libraries(TriviaGameClient PUBLIC ${Boost_SYSTEM_LIBRARY})
target_link_libraries (TriviaGameClient ${CMAKE_THREAD_LIBS_INIT})

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

