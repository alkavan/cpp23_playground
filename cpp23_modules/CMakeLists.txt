cmake_minimum_required(VERSION 3.28)
project(CppModules CXX)

set(CMAKE_CXX_STANDARD 23) # Use 23 for /std:c++latest; alternatively, set to 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions

# Find the Visual Studio toolset's module directory
# VCToolsInstallDir is typically set by the Visual Studio environment
if(DEFINED ENV{VCToolsInstallDir})
    set(VCTOOLS_DIR "$ENV{VCToolsInstallDir}")
else()
    message(FATAL_ERROR "VCToolsInstallDir environment variable not found. Ensure Visual Studio environment is set up.")
endif()

# Path to std.ixx
set(STD_MODULE_SOURCE "${VCTOOLS_DIR}/modules/std.ixx")

# Check if the module file exists
if(NOT EXISTS "${STD_MODULE_SOURCE}")
    message(FATAL_ERROR "std.ixx not found at ${STD_MODULE_SOURCE}")
endif()

# Add a custom target to compile std.ixx into an object file
add_custom_command(
        OUTPUT std.obj
        COMMAND ${CMAKE_CXX_COMPILER} /std:c++latest /EHsc /nologo /W4 /c "${STD_MODULE_SOURCE}"
        DEPENDS "${STD_MODULE_SOURCE}"
        COMMENT "Compiling C++ standard library module std.ixx"
        VERBATIM
)

# Create a static library from std.obj
add_library(std_module STATIC)
target_sources(std_module PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/std.obj")
set_target_properties(std_module PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)

add_executable(std_module_example main.cpp hello.ixx)

target_sources(std_module_example
        PRIVATE
        main.cpp
        PRIVATE
        FILE_SET CXX_MODULES
        TYPE CXX_MODULES
        FILES
        hello.ixx
        scopes.ixx
)

# Link against std_module library
target_link_libraries(std_module_example PRIVATE std_module)

# Specify module-specific compiler flags
target_compile_options(std_module_example PRIVATE /std:c++latest /experimental:module /EHsc /W4)
