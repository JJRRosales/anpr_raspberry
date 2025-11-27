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
# We assume you still have the AWS SDK installed in C:/cpp_sdk_install from before.
# If you deleted it, you will need to rebuild AWS SDK or add_subdirectory for it too.
#find_package(aws-iot-device-sdk-cpp-v2 REQUIRED)
# Find the foundational C++ common runtime
find_package(aws-crt-cpp REQUIRED CONFIG)

# Find the specific IoT services you need (e.g., Shadow and Commands)
find_package(IotShadow-cpp REQUIRED CONFIG)
find_package(IotCommands-cpp REQUIRED CONFIG)
# C) TENSORFLOW LITE (Source Integration)
# We disable the "Install" step that caused your errors
set(TFLITE_ENABLE_INSTALL OFF CACHE BOOL "Disable TFLite install" FORCE)

# We add the source directory directly. 
# Make sure this path points to 'tensorflow/lite' inside your source folder.
add_subdirectory(dependencies/tensorflow_src/tensorflow/lite)

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
    
    # Link AWS
    #AWS::aws-iot-device-sdk-cpp-v2
    
    # Link TensorFlow Lite
    # When using add_subdirectory, the target name is exactly: tensorflow-lite
    tensorflow-lite
    aws-crt-cpp::aws-crt-cpp         # Link the foundational client
    IotShadow-cpp::IotShadow-cpp     # Link the Shadow service
    IotCommands-cpp::IotCommands-cpp
)

# -----------------------------------------------------------------------------
# 6. INSTALL
# -----------------------------------------------------------------------------
install(TARGETS rpi_vision DESTINATION bin)
