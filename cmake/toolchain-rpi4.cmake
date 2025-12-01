# cmake/toolchain-rpi.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 1. Compilers
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# 2. Sysroot (System libraries)
set(RPI_SYSROOT "${CMAKE_CURRENT_LIST_DIR}/../dependencies/rpi-rootfs")
set(CMAKE_SYSROOT "${RPI_SYSROOT}")

# 3. Search Behavior (Lock it down to the sysroot)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# 4. Flags (Optional but recommended)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")