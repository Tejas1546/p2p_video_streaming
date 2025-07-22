# C++ P2P Video Streaming Platform

## Overview
This project is a peer-to-peer (P2P) video streaming platform written in C++17. It features:
- A central Tracker Server for peer discovery
- A Peer Client application for live video streaming

**Tech Stack:**
- C++17
- Poco C++ Libraries (networking)
- OpenCV (video capture)
- CMake (build system)
- vcpkg (dependency manager)

## Project Structure
```
P2P-Project/
├── TrackerServer/   # Central tracker for peer discovery
└── PeerClient/      # Main client for video streaming
```

## Prerequisites
- Visual Studio 2022 (with C++ workload) or compatible C++17 compiler
- [Git for Windows](https://gitforwindows.org/)
- [CMake](https://cmake.org/download/)
- [vcpkg](https://github.com/microsoft/vcpkg)
- (Recommended) [Visual Studio Code](https://code.visualstudio.com/) with C/C++ and CMake Tools extensions

## Setup Instructions

### 1. Clone the Repository
```sh
git clone https://github.com/Tejas1546/internship.git
cd internship
```

### 2. Install vcpkg and Dependencies
```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install poco opencv4
```

### 3. Configure the Project
- The project is set up to use the vcpkg toolchain file automatically via `.vscode/settings.json`.
- If building manually, specify the toolchain file:
  ```sh
  cmake -B P2P-Project/PeerClient/build -S P2P-Project/PeerClient -DCMAKE_TOOLCHAIN_FILE="<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake"
  cmake -B P2P-Project/TrackerServer/build -S P2P-Project/TrackerServer -DCMAKE_TOOLCHAIN_FILE="<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake"
  ```

### 4. Build
```sh
cmake --build P2P-Project/TrackerServer/build
cmake --build P2P-Project/PeerClient/build
```

## Usage
### 1. Start the Tracker Server
```sh
P2P-Project/TrackerServer/build/Debug/TrackerServer.exe
```

### 2. Start the Peer Client
```sh
P2P-Project/PeerClient/build/Debug/PeerClient.exe
```

- The Peer Client will register with the tracker and open a video capture window.
- You can run multiple Peer Clients to simulate multiple peers.

## Notes
- The `vcpkg/` directory is not included in this repository. Each user should install vcpkg locally.
- For Windows, use PowerShell or the Developer Command Prompt for all commands.
- If you encounter build issues, ensure your compiler and vcpkg are up to date.

