#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QListWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QThread>
#include <thread>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/SocketStream.h>
#include <iostream>
#include <vector>
#include <string>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>

std::vector<std::string> findPeers(const std::string& trackerIp, int trackerPort) {
    std::vector<std::string> peers;
    try {
        Poco::Net::SocketAddress addr(trackerIp, trackerPort);
        Poco::Net::StreamSocket socket(addr);
        Poco::Net::SocketStream ss(socket);
        ss << "LIST\n" << std::flush;
        std::string line;
        while (std::getline(ss, line)) {
            if (line == "END") break;
            peers.push_back(line);
        }
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] Failed to fetch peer list: " << ex.what() << std::endl;
    }
    return peers;
}

std::string getLocalIp() {
    // Get the first non-loopback IPv4 address
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces) {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            for (const QNetworkAddressEntry& entry : iface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                    !entry.ip().isLoopback()) {
                    return entry.ip().toString().toStdString();
                }
            }
        }
    }
    return "192.168.1.1";
}

void registerWithTracker(const std::string& trackerIp, int trackerPort, int listenPort) {
    try {
        Poco::Net::SocketAddress addr(trackerIp, trackerPort);
        Poco::Net::StreamSocket socket(addr);
        Poco::Net::SocketStream ss(socket);
        std::string myIp = getLocalIp();
        ss << "REGISTER " << myIp << ":" << listenPort << "\n" << std::flush;
        std::string response;
        std::getline(ss, response);
        std::cout << "[DEBUG] Tracker response: " << response << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "[DEBUG] Failed to register with tracker: " << ex.what() << std::endl;
    }
}

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);
        auto *label = new QLabel("P2P Video Streaming Client", this);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        auto *findPeersBtn = new QPushButton("Find Peers", this);
        auto *startStreamBtn = new QPushButton("Start Stream", this);
        auto *watchBtn = new QPushButton("Watch", this);
        auto *exitBtn = new QPushButton("Exit", this);

        layout->addWidget(findPeersBtn);
        layout->addWidget(startStreamBtn);
        layout->addWidget(watchBtn);
        layout->addWidget(exitBtn);

        connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
        connect(startStreamBtn, &QPushButton::clicked, this, static_cast<void (MainWindow::*)()>(&MainWindow::onStartStream));
        connect(watchBtn, &QPushButton::clicked, this, &MainWindow::onWatch);
        connect(findPeersBtn, &QPushButton::clicked, this, &MainWindow::onFindPeers);
    }

private slots:
    void onStartStream() {
        onStartStream(QString(), -1);
    }
    void onStartStream(const QString& ip, int port) {
        bool ok1 = true, ok2 = true;
        QString ipAddr = ip;
        int portNum = port;
        if (ip.isEmpty() || port == -1) {
            ipAddr = QInputDialog::getText(this, "Stream To", "Peer IP:", QLineEdit::Normal, "192.168.1.1", &ok1);
            portNum = QInputDialog::getInt(this, "Stream To", "Port:", 9000, 1, 65535, 1, &ok2);
        }
        if (!ok1 || !ok2) return;
        std::thread([ipAddr, portNum]() {
            try {
                Poco::Net::SocketAddress addr(ipAddr.toStdString(), portNum);
                Poco::Net::StreamSocket socket(addr);
                Poco::Net::SocketStream ss(socket);
                cv::VideoCapture cap(0);
                if (!cap.isOpened()) {
                    std::cerr << "[ERROR] Could not open camera." << std::endl;
                    return;
                }
                cv::Mat frame;
                while (true) {
                    cap >> frame;
                    if (frame.empty()) break;
                    int rows = frame.rows, cols = frame.cols, type = frame.type();
                    int dataSize = frame.total() * frame.elemSize();
                    ss << rows << ' ' << cols << ' ' << type << ' ' << dataSize << '\n';
                    ss.write(reinterpret_cast<const char*>(frame.data), dataSize);
                    ss.flush();
                    if (cv::waitKey(1) == 'q') break;
                }
                cap.release();
            } catch (const std::exception& ex) {
                std::cerr << "[ERROR] " << ex.what() << std::endl;
            }
        }).detach();
    }

    void onWatch() {
        bool ok;
        int port = QInputDialog::getInt(this, "Watch", "Port:", 9000, 1, 65535, 1, &ok);
        if (!ok) return;
        // Prompt for tracker IP/port to register
        QString trackerIp = QInputDialog::getText(this, "Tracker IP", "Tracker IP:", QLineEdit::Normal, "192.168.1.1", &ok);
        if (!ok) return;
        int trackerPort = QInputDialog::getInt(this, "Tracker Port", "Port:", 9000, 1, 65535, 1, &ok);
        if (!ok) return;
        // Register with tracker before starting watcher
        registerWithTracker(trackerIp.toStdString(), trackerPort, port);
        std::thread([port]() {
            try {
                Poco::Net::ServerSocket server(port);
                Poco::Net::StreamSocket client = server.acceptConnection();
                Poco::Net::SocketStream ss(client);
                int rows, cols, type, dataSize;
                while (ss >> rows >> cols >> type >> dataSize) {
                    ss.get();
                    std::vector<uchar> buf(dataSize);
                    ss.read(reinterpret_cast<char*>(buf.data()), buf.size());
                    cv::Mat frame(rows, cols, type, buf.data());
                    cv::imshow("Received Video", frame);
                    if (cv::waitKey(1) == 'q') break;
                }
                client.close();
                cv::destroyAllWindows();
            } catch (const std::exception& ex) {
                std::cerr << "[ERROR] " << ex.what() << std::endl;
            }
        }).detach();
    }

    void onFindPeers() {
        bool ok;
        QString trackerIp = QInputDialog::getText(this, "Tracker IP", "Tracker IP:", QLineEdit::Normal, "192.168.1.1", &ok);
        if (!ok) return;
        int trackerPort = QInputDialog::getInt(this, "Tracker Port", "Port:", 9000, 1, 65535, 1, &ok);
        if (!ok) return;
        auto peers = findPeers(trackerIp.toStdString(), trackerPort);
        if (peers.empty()) {
            QMessageBox::information(this, "Find Peers", "No peers found.");
            return;
        }
        QDialog dialog(this);
        dialog.setWindowTitle("Available Peers");
        QVBoxLayout vbox;
        QListWidget listWidget;
        for (const auto& peer : peers) listWidget.addItem(QString::fromStdString(peer));
        vbox.addWidget(&listWidget);
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        vbox.addWidget(&buttonBox);
        dialog.setLayout(&vbox);
        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        if (dialog.exec() == QDialog::Accepted && listWidget.currentItem()) {
            QString peerAddr = listWidget.currentItem()->text();
            // Parse IP and port from peerAddr (format: "ip:port")
            QStringList parts = peerAddr.split(":");
            if (parts.size() == 2) {
                onStartStream(parts[0], parts[1].toInt());
            }
        }
    }
};

#include <QMetaObject>
#include <QMetaMethod>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("P2P Video Streaming Client");
    w.resize(300, 250);
    w.show();
    return app.exec();
}

#include "main.moc" 