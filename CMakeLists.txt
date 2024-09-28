cmake_minimum_required(VERSION 3.15)
project(PolyformRPG)

# Set the C++ standard to the latest available (C++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # Allow compiler-specific extensions

# Automatically configure build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp" "main.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Option to set the raylib-cpp path (must be provided)
set(RAYLIB_CPP_PATH "" CACHE PATH "Path to raylib-cpp include directory")
if(NOT RAYLIB_CPP_PATH)
    message(FATAL_ERROR "The RAYLIB_CPP_PATH must be specified. Use -DRAYLIB_CPP_PATH=/path/to/raylib-cpp/include to set it.")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${RAYLIB_CPP_PATH})

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Platform-independent optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# Compiler and platform-specific optimizations
if(MSVC)
    # MSVC-specific flags
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /MP /std:c++latest /wd4244 /utf-8)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE UNICODE)
    
    # Advanced optimizations common across configurations
    target_compile_options(${PROJECT_NAME} PRIVATE /Oi /GL /Gy /fp:fast /GS- /GF /GA /arch:AVX2)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32 _DEBUG _CONSOLE)
        target_link_options(${PROJECT_NAME} PRIVATE /DEBUG /SUBSYSTEM:CONSOLE /ASAN)
        target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=address /bigobj /Zc:inline)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32 NDEBUG _CONSOLE)
        target_link_options(${PROJECT_NAME} PRIVATE /LTCG /OPT:REF /OPT:ICF /SUBSYSTEM:CONSOLE)
        target_compile_options(${PROJECT_NAME} PRIVATE /O2 /Ot /Zc:inline /GL /GS- /GF /GA)
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_PLATFORM x64)
        target_compile_options(${PROJECT_NAME} PRIVATE /favor:INTEL64)
    else()
        set(TARGET_PLATFORM Win32)
        target_compile_options(${PROJECT_NAME} PRIVATE /favor:AMD64)
    endif()

    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC and Clang-specific flags
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)
    target_compile_options(${PROJECT_NAME} PRIVATE -march=native -mtune=native -fno-plt -fdata-sections -ffunction-sections)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${PROJECT_NAME} PRIVATE -O2 -flto -fuse-linker-plugin -ffat-lto-objects -fno-rtti)
        target_link_options(${PROJECT_NAME} PRIVATE -flto)
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_options(${PROJECT_NAME} PRIVATE -D LINUX)
        target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
    endif()
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE})
