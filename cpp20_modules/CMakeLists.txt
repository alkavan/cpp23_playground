cmake_minimum_required(VERSION 3.28)
project(cpp20_modules CXX)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)
# Set the version of C++ for the project
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options("/experimental:module")
else()
    # Other compiler options
endif()

# Create a library
add_library(module_lib1)

# Add the module file to the library
target_sources(module_lib1
        PUBLIC
        FILE_SET CXX_MODULES FILES
        hello.ixx
        scopes.ixx
)

# Create an executable
add_executable(cpp20_modules main.cpp)

# Link to the library modules
target_link_libraries(cpp20_modules module_lib1)

if(MSVC)
    target_compile_options(cpp20_modules PUBLIC "/ZI")
    target_link_options(cpp20_modules PUBLIC "/INCREMENTAL")
endif()
