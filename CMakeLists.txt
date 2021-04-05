cmake_minimum_required(VERSION 3.17)
project(philosophers)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(philosophers main.cpp philosopher.cpp philosopher.h tableware.cpp tableware.h waiter.cpp waiter.h)
target_link_libraries(philosophers ${CURSES_LIBRARIES})