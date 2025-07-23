# C++ P2P Video Streaming Platform

## Overview
A cross-platform, peer-to-peer (P2P) video streaming application written in C++17 with a modern Qt GUI. Stream live video directly between two computers on the same network, with an optional tracker server for peer discovery.

---

## Features
- Live video streaming between two peers
- Simple, user-friendly Qt GUI
- Optional tracker server for peer registration
- Windows installer (no need to install Qt/OpenCV/Poco separately)

---

## Project Structure
```
P2P-Project/
├── TrackerServer/   # Central tracker for peer discovery
└── PeerClient/      # Main client for video streaming
```

---

## Tech Stack
- C++17
- Qt 6 (GUI)
- Poco (networking)
- OpenCV (video capture)
- CMake (build system)
- vcpkg (dependency manager)

---

## Prerequisites
- Visual Studio 2022 (with C++ workload) or compatible C++17 compiler
- [Git for Windows](https://gitforwindows.org/)
- [CMake](https://cmake.org/download/)
- [vcpkg](https://github.com/microsoft/vcpkg)
- (Recommended) [Visual Studio Code](https://code.visualstudio.com/) with C/C++ and CMake Tools extensions

---

## Installation (Windows)
1. **Download and run the installer** (`PeerClientSetup.exe`).
2. Follow the prompts to install PeerClient.
3. (Optional) Install the TrackerServer if you want to use peer discovery.

---

## Usage
### 1. Start the Tracker Server (Optional)
- Run `TrackerServer.exe` (from its build or install location).
- The server listens on port 9000 by default.

### 2. Launch PeerClient
- Run `PeerClient.exe` (from the Start Menu, Desktop, or install folder).
- The Qt window will appear with three buttons: **Start Stream**, **Watch**, **Exit**.

### 3. To Receive Video (Watcher)
- Click **Watch**.
- Enter a port (e.g., `9000`).
- The app will wait for an incoming video connection.

### 4. To Send Video (Streamer)
- Click **Start Stream**.
- Enter the IP address of the receiver (e.g., `192.168.1.42` for LAN, or `127.0.0.1` for local test).
- Enter the same port (e.g., `9000`).
- Your camera feed will be sent to the receiver.

### 5. Stopping
- Close the OpenCV video window or press `q` in the video window to stop streaming/watching.
- Click **Exit** in the main window to close the app.

---

## LAN Setup Example
| Role      | Machine | Action         | IP/Port to Use         |
|-----------|---------|---------------|------------------------|
| Watcher   | PC 1    | Watch         | Port: 9000             |
| Streamer  | PC 2    | Start Stream  | IP: PC 1's LAN IP<br>Port: 9000 |

- Find your LAN IP with `ipconfig` (Windows) or `ifconfig` (Mac/Linux).
- Both machines must be on the same network.
- Allow the app through the firewall if prompted.

---

## Troubleshooting / FAQ
- **Missing DLL error:** Ensure all DLLs and the `platforms` folder are present next to `PeerClient.exe`.
- **Qt platform plugin error:** Make sure `platforms/qwindows.dll` is present.
- **Connection refused:** Make sure the watcher is running and listening on the correct port, and the firewall allows connections.
- **No video:** Ensure your camera is connected and not in use by another app.
- **LAN streaming not working:** Double-check both machines are on the same network and can ping each other. Temporarily disable firewalls for testing.
- **How do I use on Mac/Linux?**: You must build from source and bundle dependencies for those platforms (see below).

---

## Building from Source (Developers)
1. **Clone the repository**
2. **Install dependencies:**
   - Qt 6.9.1 (MSVC 2022 64-bit)
   - OpenCV (via vcpkg or system)
   - Poco (via vcpkg or system)
3. **Configure and build:**
   ```sh
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release
   ```
4. **Deploy:**
   - Use `windeployqt` for Qt DLLs
   - Copy OpenCV and Poco DLLs from vcpkg

---

## License
MIT (or your chosen license)

---

## Credits
- Built with Qt, OpenCV, Poco, and CMake
- Created by Tejas (and contributors)

