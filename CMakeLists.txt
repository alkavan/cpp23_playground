cmake_minimum_required(VERSION 3.28)
project(cpp23_playground CXX)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)
# Set the version of C++ for the project
set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options("/experimental:module")
else()
    # Other compiler options
endif()

# Create a library
add_library(modules)

# Add the module file to the library
target_sources(modules
        PUBLIC
        FILE_SET CXX_MODULES FILES
        hello.ixx
        scopes.ixx
)

# Create an executable
add_executable(playground main.cpp)

# Link to the library modules
target_link_libraries(playground modules)

if(MSVC)
    target_compile_options(playground PUBLIC "/ZI")
    target_link_options(playground PUBLIC "/INCREMENTAL")
endif()
