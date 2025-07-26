# Tejas J Shetty_Sahyadri College Of Engieering And Management_C++_10

# P2P Video Streaming Platform

A C++ peer-to-peer video streaming platform that allows users to share and watch live video streams with low latency. Built with Poco C++ libraries for efficient networking and OpenCV for video processing.

## Features

- **Live Video Streaming**: Stream video from your camera to other peers
- **Screen Sharing**: Share your entire desktop screen with other peers
- **Peer Discovery**: Find and connect to other peers on the network via TrackerServer
- **Low Latency**: Optimized for real-time video communication
- **Cross-Platform GUI**: Modern Qt-based interface for easy interaction
- **LAN Support**: Designed for local network video streaming
- **JPEG Compression**: Efficient video compression for better quality and bandwidth usage

## Tech Stack

- **C++17**: Core programming language
- **Poco C++ Libraries**: Networking and communication
- **OpenCV**: Video capture, processing, and display
- **Qt 6**: Cross-platform GUI framework
- **CMake**: Build system
- **vcpkg**: Package management
- **Windows GDI+**: Screen capture functionality

## Project Structure

```
Internship-task/
├── dev/                          # Development source code
│   ├── P2P-Project/
│   │   ├── PeerClient/           # Main peer client application
│   │   │   ├── main.cpp          # Qt GUI and P2P streaming logic
│   │   │   └── CMakeLists.txt    # Build configuration
│   │   └── TrackerServer/        # Central peer discovery server
│   │       ├── main.cpp          # Server implementation
│   │       └── CMakeLists.txt    # Build configuration
│   ├── vcpkg/                    # Package manager (git submodule)
│   └── .gitignore                # Git ignore rules
└── installer/                    # Distribution installers
    ├── PeerClientSetup.exe       # PeerClient installer
    └── TrackerServerSetup.exe    # TrackerServer installer

```

## Prerequisites

- Windows 10/11 (64-bit)
- Visual Studio 2019/2022 with MSVC compiler
- Git
- Qt 6.9.1 (MSVC 2022 64-bit)
- Internet connection for initial setup

## User Manual

### TrackerServer

1. **Installation**: Run `TrackerServerSetup.exe` and follow the installation wizard
2. **Starting**: Launch TrackerServer from Start Menu or Desktop shortcut
3. **Usage**: The server runs in the background and handles peer registration
4. **Configuration**: Default port is 9000 (can be changed in source code)

### PeerClient

1. **Installation**: Run `PeerClientSetup.exe` and follow the installation wizard
2. **Starting**: Launch PeerClient from Start Menu or Desktop shortcut
3. **Features**:
   - **Find Peers**: Discover other peers on the network via TrackerServer
   - **Start Stream**: Stream your camera to another peer
   - **Share Screen**: Stream your entire desktop screen to another peer (with JPEG compression for better quality)
   - **Watch**: Receive and display video streams from other peers
   - **Exit**: Close the application

## LAN Usage Example

**Setup**:
- PC1: Run TrackerServer on port 9000
- PC1: Run PeerClient and use "Watch" on port 10000
- PC2: Run PeerClient and use "Find Peers" to discover PC1
- PC2: Use "Start Stream" or "Share Screen" to send video to PC1

**Important**: TrackerServer and PeerClient (watcher) must use different ports on the same machine.

## Troubleshooting / FAQ

**Q: Missing DLL errors when running the installer?**
A: The installer includes all necessary dependencies. If issues persist, try running as administrator.

**Q: Qt platform plugin "windows" not found?**
A: This is usually resolved by the installer. If it occurs, ensure the application is installed correctly.

**Q: Connection refused when trying to connect?**
A: Ensure TrackerServer is running and the correct IP/port is used. Check Windows Firewall settings.

**Q: No video appears when streaming?**
A: Check camera permissions and ensure the target peer is running in "Watch" mode.

**Q: Screen sharing is laggy or poor quality?**
A: The application uses JPEG compression (80% quality) and 1280x720 resolution. Adjust these settings in the source code if needed.

**Q: Can't minimize the video window?**
A: The video window is now resizable and can be minimized/maximized as needed.

**Q: PeerClient connects to TrackerServer instead of watcher?**
A: This happens when both TrackerServer and PeerClient use the same port. Use different ports (e.g., TrackerServer: 9000, PeerClient watcher: 10000).

**Q: Works on LAN but not over internet?**
A: Internet streaming requires port forwarding on routers and public IP addresses. This application is designed for LAN usage.

## Building from Source

### Prerequisites Setup

1. **Install Visual Studio 2019/2022** with MSVC compiler
2. **Install Qt 6.9.1** (MSVC 2022 64-bit) from [Qt Downloads](https://www.qt.io/download)
3. **Clone the repository**:
   ```bash
   git clone https://github.com/Tejas1546/internship.git
   cd Internship-task/dev
   ```

### Build Instructions

1. **Setup vcpkg**:
   ```bash
   git submodule update --init --recursive
   .\vcpkg\bootstrap-vcpkg.bat
   .\vcpkg\vcpkg install poco[net] opencv4[core,imgproc,highgui,imgcodecs]
   ```

2. **Build TrackerServer**:
   ```bash
   cmake -B P2P-Project/TrackerServer/build -S P2P-Project/TrackerServer -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build P2P-Project/TrackerServer/build --config Release
   ```

3. **Build PeerClient**:
   ```bash
   cmake -B P2P-Project/PeerClient/build -S P2P-Project/PeerClient -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build P2P-Project/PeerClient/build --config Release
   ```

4. **Deploy with Qt dependencies**:
   ```bash
   windeployqt.exe P2P-Project/PeerClient/build/Release/PeerClient.exe
   ```

## Credits

**Completed as part of the MatreComm internship selection process**

This project demonstrates advanced C++ development skills including:
- Network programming with Poco C++
- Video processing with OpenCV
- GUI development with Qt
- Multi-threading and real-time communication
- Cross-platform development considerations


