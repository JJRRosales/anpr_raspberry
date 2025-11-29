cmake_minimum_required(VERSION 3.16)
project(anpr_raspberry LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# 1. VCPKG SETUP
# -----------------------------------------------------------------------------
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# -----------------------------------------------------------------------------
# 2. FIND PACKAGES
# -----------------------------------------------------------------------------

# A) Vcpkg Dependencies
find_package(OpenCV REQUIRED)
find_package(Tesseract REQUIRED)
find_package(Threads REQUIRED)

# B) AWS IoT Device SDK v2 (Manual Install)
find_package(aws-crt-cpp REQUIRED CONFIG)
find_package(IotShadow-cpp REQUIRED CONFIG)
find_package(IotCommands-cpp REQUIRED CONFIG)

# C) TENSORFLOW LITE (Source Integration)

set(TF_SRC "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tensorflow_src")

# 1. Import the Shared Library (.so) you built with Bazel
add_library(libtflite SHARED IMPORTED)

set_target_properties(libtflite PROPERTIES
    # Point this to where Bazel put the output
    IMPORTED_LOCATION "${TF_SRC}/bazel-bin/tensorflow/lite/libtensorflowlite.so"
)

# -----------------------------------------------------------------------------
# 3. BUILD TARGET
# -----------------------------------------------------------------------------
add_executable(rpi_vision
    src/main.cpp
)

# -----------------------------------------------------------------------------
# 4. INCLUDE DIRECTORIES
# -----------------------------------------------------------------------------
target_include_directories(rpi_vision PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${Tesseract_INCLUDE_DIRS}
    "${TF_SRC}"
    "${TF_SRC}/bazel-tensorflow_src/external/flatbuffers/include"
    # TFLite handles its own includes via the target below
)

# -----------------------------------------------------------------------------
# 5. LINK LIBRARIES
# -----------------------------------------------------------------------------
target_link_libraries(rpi_vision
    PRIVATE
    ${OpenCV_LIBS}
    libtesseract
    Threads::Threads
    tensorflow-lite
    aws-crt-cpp         
    IotShadow-cpp    
    IotCommands-cpp
)

# -----------------------------------------------------------------------------
# 6. INSTALL
# -----------------------------------------------------------------------------
install(TARGETS rpi_vision DESTINATION bin)
