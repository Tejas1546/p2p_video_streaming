#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Mutex.h>
#include <Poco/Timestamp.h>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <thread>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

struct PeerInfo {
    std::string address;
    Poco::Timestamp lastSeen;
};

std::unordered_map<std::string, PeerInfo> peers;
Poco::Mutex peersMutex;

void handleClient(StreamSocket client) {
    std::string peerAddr = client.peerAddress().toString();
    std::cout << "[DEBUG] Client connected: " << peerAddr << std::endl << std::flush;
    SocketStream ss(client);
    std::string line;
    while (std::getline(ss, line)) {
        std::cout << "[DEBUG] Received from " << peerAddr << ": '" << line << "'" << std::endl << std::flush;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd == "REGISTER") {
            {
                Poco::Mutex::ScopedLock lock(peersMutex);
                peers[peerAddr] = {peerAddr, Poco::Timestamp()};
            }
            ss << "OK REGISTERED\n" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Give client time to read
        } else if (cmd == "LIST") {
            Poco::Mutex::ScopedLock lock(peersMutex);
            for (const auto& [addr, info] : peers) {
                ss << addr << "\n";
            }
            ss << "END\n" << std::flush;
        } else if (cmd == "EXIT") {
            {
                Poco::Mutex::ScopedLock lock(peersMutex);
                peers.erase(peerAddr);
            }
            ss << "BYE\n" << std::flush;
            break;
        } else {
            ss << "ERR Unknown command\n" << std::flush;
        }
    }
    std::cout << "[DEBUG] Client disconnected: " << peerAddr << std::endl << std::flush;
    client.close();
}

int main() {
    const int port = 9000;
    ServerSocket server(port);
    std::cout << "TrackerServer listening on port " << port << std::endl;
    while (true) {
        StreamSocket client = server.acceptConnection();
        std::thread t(handleClient, std::move(client));
        t.detach();
    }
    return 0;
} 