# cmake/toolchain-rpi.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Use GCC 11 (matches Ubuntu 22.04 / GLIBC 2.35)
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc-11)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++-11)

# Sysroot from RPi
set(RPI_SYSROOT "${CMAKE_CURRENT_LIST_DIR}/../dependencies/rpi-rootfs")
set(CMAKE_SYSROOT "${RPI_SYSROOT}")

# Explicitly pass sysroot to compiler/linker
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${RPI_SYSROOT}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --sysroot=${RPI_SYSROOT}")

# Search behavior
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Linker flags
set(RPATH_LIB_DIR "${RPI_SYSROOT}/usr/lib/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--rpath-link=${RPATH_LIB_DIR}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--rpath-link=${RPATH_LIB_DIR}")