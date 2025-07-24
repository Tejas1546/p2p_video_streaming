# C++ P2P Video Streaming Platform

## Overview
This project is a peer-to-peer (P2P) video streaming platform for Windows, enabling users to share and watch live video streams with low latency over a local network (LAN). The platform leverages Poco C++ libraries for efficient peer-to-peer communication, OpenCV for video capture, and Qt for a modern, user-friendly GUI.

---

## Features
- Live video streaming between two peers on the same LAN
- Simple, intuitive Qt GUI for starting, watching, and discovering streams
- Peer discovery via a central TrackerServer
- Windows installer for easy setup (no need to install dependencies manually)
- Low-latency, real-time video communication

---

## Tech Stack
- **C++17**
- **Poco C++ Libraries** (networking, peer discovery)
- **OpenCV** (video capture and display)
- **Qt 6** (GUI)
- **CMake** (build system)
- **vcpkg** (dependency manager)

---

## Project Structure
```
Internship-Task/
├── dev/           # Full development workspace (source code, CMake, etc.)
├── installer/     # Windows installers for end users
│   ├── PeerClientSetup.exe
│   └── TrackerServerSetup.exe
└── README.md      # This documentation
```
- **installer/** contains ready-to-use installers for both PeerClient and TrackerServer. End users should use these to install the applications.
- **dev/** contains all source code and build scripts for developers.

---

## Prerequisites (for End Users)
- **Windows 10/11 (x64)**
- No manual dependency installation required—just use the provided installers.
- For LAN streaming, all PCs must be on the same local network.

---

## User Manual
### **1. Installing the Applications**
- Run `PeerClientSetup.exe` to install the PeerClient GUI.
- Run `TrackerServerSetup.exe` to install the TrackerServer (only needed on one PC per LAN).

### **2. Starting the TrackerServer**
- On one PC in your LAN, run `TrackerServer.exe` (from the Start Menu or install folder).
- The server listens on port 9000 by default.

### **3. Watching a Stream (PeerClient as Watcher)**
- On any PC, run `PeerClient.exe`.
- Click **Watch**.
- **Important:** Enter a port that is **different from the TrackerServer's port** (e.g., use 10000 if TrackerServer is on 9000).
- Enter the TrackerServer's IP and port (e.g., `192.168.1.10`, `9000`).
- The app will register itself with the tracker and wait for an incoming video connection.

### **4. Starting a Stream (PeerClient as Streamer)**
- On another PC, run `PeerClient.exe`.
- Click **Find Peers** to get a list of available watchers from the tracker.
- Select a peer from the list to auto-fill the IP/port, or use **Start Stream** to manually enter the IP/port.
- Your camera feed will be sent to the receiver.

### **5. Stopping**
- Close the OpenCV video window or press `q` in the video window to stop streaming/watching.
- Click **Exit** in the main window to close the app.

---

## LAN Usage Example
| Role         | Machine | Action         | IP/Port to Use         |
|--------------|---------|---------------|------------------------|
| Tracker      | PC 1    | TrackerServer | Port: 9000             |
| Watcher      | PC 1/2  | Watch         | Port: 10000            |
| Streamer     | PC 2/1  | Start Stream  | IP: Watcher's LAN IP<br>Port: 10000 |

- **Do not use the same port for TrackerServer and PeerClient (watcher) on the same machine.**
- Find your LAN IP with `ipconfig` (Windows).
- Both machines must be on the same network.
- Allow the app through the firewall if prompted.

---

## Troubleshooting / FAQ
- **Missing DLL error:** Ensure all DLLs and the `platforms` folder are present next to `PeerClient.exe`.
- **Qt platform plugin error:** Make sure `platforms/qwindows.dll` is present.
- **Connection refused:** Make sure the watcher is running and listening on the correct port, and the firewall allows connections.
- **No video:** Ensure your camera is connected and not in use by another app.
- **LAN streaming not working:** Double-check both machines are on the same network and can ping each other. Temporarily disable firewalls for testing.
- **PeerClient connects to TrackerServer instead of watcher:** Make sure the watcher is using a different port than the TrackerServer. Register the watcher with its own listening port (e.g., 10000), not the tracker’s port (9000).
- **How do I use on Mac/Linux?**: This release is for Windows only. For other platforms, you must build from source and bundle dependencies.

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
   - Use Inno Setup to create Windows installers

---

## Credits
- Built with Qt, OpenCV, Poco, and CMake
- Created by Tejas
- Completed as part of the MatreComm internship selection process


