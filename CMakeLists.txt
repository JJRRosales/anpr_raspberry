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
find_package(Tesseract CONFIG REQUIRED)
find_package(Threads REQUIRED)

# B) AWS IoT Device SDK v2 (Manual Install)
set(AWS_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/aws-install")

find_library(AWS_CRT_CPP_LIB 
    NAMES aws-crt-cpp
    PATHS "${AWS_INSTALL_DIR}/lib"
    REQUIRED
)

find_library(AWS_IOT_SHADOW_LIB
    NAMES IotShadow-cpp
    PATHS "${AWS_INSTALL_DIR}/lib"
    REQUIRED
)

find_library(AWS_IOT_COMMANDS_LIB
    NAMES IotCommands-cpp
    PATHS "${AWS_INSTALL_DIR}/lib"
    REQUIRED
)

find_library(AWS_IOT_JOBS_LIB
    NAMES IotJobs-cpp
    PATHS "${AWS_INSTALL_DIR}/lib"
    REQUIRED
)

find_library(AWS_IOT_IDENTITY_LIB
    NAMES IotIdentity-cpp
    PATHS "${AWS_INSTALL_DIR}/lib"
    REQUIRED
)

find_library(AWS_IOT_COMMON_LIB
    NAMES IotDeviceCommon-cpp
    PATHS "${AWS_INSTALL_DIR}/lib"
    REQUIRED
)

set(AWS_IOT_INCLUDE_DIRS
    "${AWS_INSTALL_DIR}/include"
)

# C) TENSORFLOW LITE 
set(TF_SRC "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tensorflow_src")

add_library(libtflite SHARED IMPORTED)

set_target_properties(libtflite PROPERTIES
    IMPORTED_LOCATION "${TF_SRC}/bazel-bin/tensorflow/lite/libtensorflowlite.so"
    INTERFACE_INCLUDE_DIRECTORIES "${TF_SRC};${TF_SRC}/bazel-tensorflow_src/external/flatbuffers/include"
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
target_include_directories(rpi_vision PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${AWS_IOT_INCLUDE_DIRS}
)

# -----------------------------------------------------------------------------
# 5. LINK LIBRARIES
# -----------------------------------------------------------------------------
target_link_libraries(rpi_vision
    PRIVATE
    ${OpenCV_LIBS}          
    Tesseract::libtesseract
    Threads::Threads         
    libtflite                
    ${AWS_CRT_CPP_LIB}
    ${AWS_IOT_SHADOW_LIB}
    ${AWS_IOT_COMMANDS_LIB}
    ${AWS_IOT_JOBS_LIB}
    ${AWS_IOT_IDENTITY_LIB}
    ${AWS_IOT_COMMON_LIB}
)

target_link_directories(rpi_vision PRIVATE
    "${AWS_INSTALL_DIR}/lib"
)
# -----------------------------------------------------------------------------
# 6. INSTALL
# -----------------------------------------------------------------------------
install(TARGETS rpi_vision DESTINATION bin)
