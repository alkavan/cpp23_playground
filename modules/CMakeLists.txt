cmake_minimum_required(VERSION 3.28)
project(CppModules CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions

# Check compiler and platform
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Windows with MSVC - use full module support
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
    
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
    
    add_executable(std_module_example main.cpp)
    
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
    
    target_link_libraries(std_module_example PRIVATE std_module)
    target_compile_options(std_module_example PRIVATE /std:c++latest /experimental:module /EHsc /W4)
    
else()
    # Non-MSVC compilers (Clang, GCC) - treat modules as regular source files
    message(STATUS "Using ${CMAKE_CXX_COMPILER_ID} compiler - treating modules as source files")
    
    add_executable(std_module_example 
        main.cpp
        hello.ixx
        scopes.ixx
    )

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
    
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        # Check Clang version for module support
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "16.0")
            message(STATUS "Clang ${CMAKE_CXX_COMPILER_VERSION} detected - enabling experimental module support")
            target_compile_options(std_module_example PRIVATE 
                -std=c++23 
                -Wall 
                -Wextra
                -fmodules
                -fbuiltin-module-map
            )
        else()
            message(WARNING "Clang version ${CMAKE_CXX_COMPILER_VERSION} may have limited module support")
            target_compile_options(std_module_example PRIVATE 
                -std=c++23 
                -Wall 
                -Wextra
            )
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # GCC modules support is experimental
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "11.0")
            message(WARNING "GCC version ${CMAKE_CXX_COMPILER_VERSION} may have limited C++20/23 modules support")
        endif()
        
        target_compile_options(std_module_example PRIVATE 
            -std=c++23 
            -Wall 
            -Wextra
        )
    else()
        message(WARNING "Unknown compiler ${CMAKE_CXX_COMPILER_ID}, using default C++23 settings")
        target_compile_options(std_module_example PRIVATE -std=c++23 -Wall -Wextra)
    endif()
    
endif()

# Additional settings for specific platforms
if(APPLE)
    # macOS specific settings
    set_target_properties(std_module_example PROPERTIES
        MACOSX_RPATH TRUE
    )
endif()
