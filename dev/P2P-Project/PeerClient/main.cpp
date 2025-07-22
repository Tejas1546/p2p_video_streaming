#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/SocketStream.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

void registerWithTracker(const std::string& host, int port) {
    std::cout << "[DEBUG] Attempting to connect to tracker at " << host << ":" << port << std::endl << std::flush;
    try {
        Poco::Net::SocketAddress addr(host, port);
        Poco::Net::StreamSocket socket(addr);
        Poco::Net::SocketStream ss(socket);
        ss << "REGISTER\n" << std::flush;
        std::string response;
        std::getline(ss, response);
        std::cout << "[DEBUG] Tracker response: '" << response << "'" << std::endl << std::flush;
        std::cout << "[DEBUG] Finished reading tracker response." << std::endl << std::flush;
    } catch (const std::exception& ex) {
        std::cerr << "[DEBUG] Failed to register with tracker: " << ex.what() << std::endl << std::flush;
    }
    std::cout << "[DEBUG] Finished tracker registration attempt." << std::endl << std::flush;
}

int main() {
    std::cout << "[DEBUG] PeerClient starting..." << std::endl << std::flush;
    registerWithTracker("127.0.0.1", 9000);
    std::cout << "[DEBUG] Starting video capture..." << std::endl << std::flush;
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "[DEBUG] Error: Could not open camera." << std::endl << std::flush;
        return 1;
    }
    cv::Mat frame;
    std::cout << "[DEBUG] Entering video loop. Press 'q' to exit." << std::endl << std::flush;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "[DEBUG] Error: Blank frame grabbed." << std::endl << std::flush;
            break;
        }
        cv::imshow("Camera", frame);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    std::cout << "[DEBUG] PeerClient exiting." << std::endl << std::flush;
    return 0;
} 