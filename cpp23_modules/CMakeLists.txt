cmake_minimum_required(VERSION 3.28)
project(Cpp23_Modules CXX)

# turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options("/experimental:module")
else()
    # other compiler options
endif()

# you don't want to hardcode the path here in a real-world project, but you get the idea...
set(MSVC_MODULE_PATH "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.39.33519/modules/")

# Not sure suddenly this does not copy?

#file(COPY
#        "${MSVC_MODULE_PATH}/std.ixx"
#        DESTINATION
#        ${PROJECT_SOURCE_DIR}
#)

# Create a library
add_library(module_lib1)

# Add the module file to the library
target_sources(module_lib1
        PUBLIC
        FILE_SET CXX_MODULES FILES
        std.ixx
        hello.ixx
        scopes.ixx
)

# Create an executable
add_executable(modules_example main.cpp)

# Link to the library modules
target_link_libraries(modules_example module_lib1)

if(MSVC)
    target_compile_options(modules_example PUBLIC "/Zi")
    target_link_options(modules_example PUBLIC "/INCREMENTAL")
endif()
