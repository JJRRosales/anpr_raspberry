cmake_minimum_required(VERSION 3.16)
project(anpr_raspberry LANGUAGES CXX)

# =========================================================
#  DEPLOYMENT CONFIGURATION
# =========================================================
set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# -----------------------------------------------------------------------------
# 1. FIND PACKAGES
# -----------------------------------------------------------------------------
# VCPKG libs
find_package(OpenCV REQUIRED)
find_package(Tesseract CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Setup Paths for Manual Libs
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Configuring for RASPBERRY PI (Cross-Compile)")
    set(AWS_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/aws-install-rpi") 
    set(TFLITE_LIB "${CMAKE_SOURCE_DIR}/dependencies/tflite/aarch64/libtensorflowlite.so") 
else()
    message(STATUS "Configuring for LOCAL HOST (x64)")
    set(AWS_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/aws-install")
    set(TFLITE_LIB "${CMAKE_SOURCE_DIR}/dependencies/tflite/x86_64/libtensorflowlite.so")
endif()

# AWS Libs
find_library(AWS_CRT_CPP_LIB NAMES aws-crt-cpp PATHS "${AWS_INSTALL_DIR}/lib" REQUIRED NO_CMAKE_FIND_ROOT_PATH)
find_library(AWS_IOT_SHADOW_LIB NAMES IotShadow-cpp PATHS "${AWS_INSTALL_DIR}/lib" REQUIRED NO_CMAKE_FIND_ROOT_PATH)
find_library(AWS_IOT_COMMANDS_LIB NAMES IotCommands-cpp PATHS "${AWS_INSTALL_DIR}/lib" REQUIRED NO_CMAKE_FIND_ROOT_PATH)
find_library(AWS_IOT_JOBS_LIB NAMES IotJobs-cpp PATHS "${AWS_INSTALL_DIR}/lib" REQUIRED NO_CMAKE_FIND_ROOT_PATH)
find_library(AWS_IOT_IDENTITY_LIB NAMES IotIdentity-cpp PATHS "${AWS_INSTALL_DIR}/lib" REQUIRED NO_CMAKE_FIND_ROOT_PATH)
find_library(AWS_IOT_COMMON_LIB NAMES IotDeviceCommon-cpp PATHS "${AWS_INSTALL_DIR}/lib" REQUIRED NO_CMAKE_FIND_ROOT_PATH)


# TFLite
add_library(libtflite SHARED IMPORTED)
set_target_properties(libtflite PROPERTIES
    IMPORTED_LOCATION "${TFLITE_LIB}"
    INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tensorflow_src;${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tensorflow_src/bazel-tensorflow_src/external/flatbuffers/include"
)



# -----------------------------------------------------------------------------
# 2. BUILD TARGET
# -----------------------------------------------------------------------------
add_executable(rpi_vision src/main.cpp src/Config.cpp)

target_include_directories(rpi_vision PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${AWS_INSTALL_DIR}/include"
)

target_link_libraries(rpi_vision PRIVATE
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

# -----------------------------------------------------------------------------
# 3. INSTALL
# -----------------------------------------------------------------------------
# A. The Binary
install(TARGETS rpi_vision DESTINATION .)
# B. Manual Libs (AWS + TFLite)
file(GLOB AWS_LIBS "${AWS_INSTALL_DIR}/lib/*.so*")
install(FILES ${AWS_LIBS} DESTINATION .)
install(FILES "${TFLITE_LIB}" DESTINATION .)
# VCPK install
# Install vcpkg-managed libraries
install(DIRECTORY "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/lib/"
        DESTINATION lib
        FILES_MATCHING PATTERN "*.so*")
