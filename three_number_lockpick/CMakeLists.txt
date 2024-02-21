cmake_minimum_required(VERSION 3.28)
project(cpp20_modules CXX)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)
# Set the version of C++ for the project
set(CMAKE_CXX_STANDARD 23)

# Create an executable
add_executable(three_number_lockpick
        main.cpp
)
