
# Build & Install Guide (CMake + vcpkg)

---

## Requirements

| Platform | Required Tools |
| :--- | :--- |
| **Windows 10/11** | Visual Studio 2022, CMake, Git, vcpkg |
| **Ubuntu / Raspberry Pi (Ubuntu)** | build-essential, CMake, Git, vcpkg |
| **Every platform** | Internet (for downloading dependencies) |

---

## 1) Install vcpkg

### Windows

```bash
git clone [https://github.com/microsoft/vcpkg.git](https://github.com/microsoft/vcpkg.git)
cd vcpkg
bootstrap-vcpkg.bat
````

### Linux / Raspberry Pi Ubuntu

```bash
git clone [https://github.com/microsoft/vcpkg.git](https://github.com/microsoft/vcpkg.git)
cd vcpkg
./bootstrap-vcpkg.sh
```

-----

## 2\) Integrate vcpkg

> This enables automatic detection of vcpkg packages in CMake/Visual Studio.

### Windows

```bash
.\vcpkg integrate install
```

### Linux / Pi

```bash
./vcpkg integrate install
```
-----

## 2.1\) Install Manual Dependencies

AWS IoT Device SDK for C++ v2

| Step | Command (Linux/Ubuntu) | Command (Windows - Developer PowerShell/CMD) |
|------|------------------------|---------------------------------------------|
| Clone & Setup | `git clone --recursive https://github.com/aws/aws-iot-device-sdk-cpp-v2.git` | `git clone --recursive https://github.com/aws/aws-iot-device-sdk-cpp-v2.git` |
| Build Directory | `mkdir aws-build && cd aws-build` | `mkdir aws-build; cd aws-build` |
| Configure | `cmake -DCMAKE_INSTALL_PREFIX="/opt/aws-iot-sdk" ../aws-iot-device-sdk-cpp-v2` | `cmake -DCMAKE_INSTALL_PREFIX="C:/AWS-SDK/v2" ../aws-iot-device-sdk-cpp-v2` |
| Build & Install | `cmake --build . --target install` | `cmake --build . --target install --config Release` |

TensorFlow Lite C++

| Step | Command (Linux/Ubuntu) | Command (Windows - Developer PowerShell/CMD) |
|------|------------------------|---------------------------------------------|
| Clone Repo | `git clone https://github.com/tensorflow/tensorflow.git tensorflow_src` | `git clone https://github.com/tensorflow/tensorflow.git tensorflow_src` |
| Build Directory | `mkdir tflite-build && cd tflite-build` | `mkdir tflite-build; cd tflite-build` |
| Configure | `cmake -DTFLITE_ENABLE_INSTALL=ON -DCMAKE_INSTALL_PREFIX="/usr/local" ../tensorflow_src/tensorflow/lite` | `cmake -DTFLITE_ENABLE_INSTALL=ON -DCMAKE_INSTALL_PREFIX="C:/TFLite-SDK" ../tensorflow_src/tensorflow/lite` |
| Build & Install | `cmake --build . --target install` | `cmake --build . --target install --config Release` |

-----

## 3\) Clone the Project

```bash
git clone <your-repo-url>
cd <project-folder>
```

Check that **vcpkg.json** exists in the project root.



-----

## 4\) Configure with CMake Presets

Your `CMakePresets.json` should include a vcpkg toolchain reference, for example:

```json
"cacheVariables": {
  "CMAKE_TOOLCHAIN_FILE": "${sourceDir}/vcpkg/scripts/buildsystems/vcpkg.cmake"
}
```

> Presets decide build type and architecture automatically. If you don’t have presets, skip to manual commands below.

### Windows Build

```bash
cmake --preset windows-debug
cmake --build --preset windows-debug
```

### Ubuntu Build

```bash
cmake --preset linux-release
cmake --build --preset linux-release
```

### Raspberry Pi Build

```bash
cmake --preset pi-release
cmake --build --preset pi-release
```

-----

## 5\) Manual Build (If You Don’t Use Presets)

### Windows (x64)

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build --config Release
```

### Linux / Raspberry Pi

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

-----

## 6\) Test the Binary

### Windows:

```bash
.\build\Release\raspberry_vision_iot.exe
```

### Linux / Pi:

```bash
./build/raspberry_vision_iot
```

-----

## Additional Notes (for this project)

### If OpenCV cannot find a camera (Ubuntu/Pi)

Test that your camera works:

```bash
v4l2-ctl --list-devices
```

Use `/dev/video0` or similar.

### Tesseract Language Pack Installation (Linux/Pi)

Install English:

```bash
sudo apt install tesseract-ocr-eng
```

To add Spanish:

```bash
sudo apt install tesseract-ocr-spa
```

### AWS IoT Certificates

Place your certificates in:

`config/certs/`

Ensure this file is configured:

`config/aws_config.json`

