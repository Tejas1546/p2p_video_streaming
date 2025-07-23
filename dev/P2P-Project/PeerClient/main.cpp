#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QThread>
#include <thread>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/SocketStream.h>
#include <iostream>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);
        auto *label = new QLabel("P2P Video Streaming Client", this);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        auto *startStreamBtn = new QPushButton("Start Stream", this);
        auto *watchBtn = new QPushButton("Watch", this);
        auto *exitBtn = new QPushButton("Exit", this);

        layout->addWidget(startStreamBtn);
        layout->addWidget(watchBtn);
        layout->addWidget(exitBtn);

        connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
        connect(startStreamBtn, &QPushButton::clicked, this, &MainWindow::onStartStream);
        connect(watchBtn, &QPushButton::clicked, this, &MainWindow::onWatch);
    }

private slots:
    void onStartStream() {
        bool ok1, ok2;
        QString ip = QInputDialog::getText(this, "Stream To", "Peer IP:", QLineEdit::Normal, "127.0.0.1", &ok1);
        int port = QInputDialog::getInt(this, "Stream To", "Port:", 9000, 1, 65535, 1, &ok2);
        if (!ok1 || !ok2) return;
        std::thread([ip, port]() {
            try {
                Poco::Net::SocketAddress addr(ip.toStdString(), port);
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
};

#include <QMetaObject>
#include <QMetaMethod>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("P2P Video Streaming Client");
    w.resize(300, 200);
    w.show();
    return app.exec();
}

#include "main.moc" 