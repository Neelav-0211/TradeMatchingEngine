cmake_minimum_required(VERSION 3.14)
project(TradeMatchingEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for performance optimization
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Testing with Google Test
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
