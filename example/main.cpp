//
// Created by Mihir Patil on 8/6/23.
//

#include <iostream>
#include "main.h"
#include <csignal>

using std::cout;
using std::endl;

void slamCallback(const xv::Pose&pose) {
    cout << pose.timestamp << ", " << pose.position[0] << ", " << pose.position[1] << ", " << pose.position[2] << endl;
}

namespace {
    volatile std::sig_atomic_t gSignalStatus;
}

void signal_handler(int signal) {
    gSignalStatus = signal;
}

int main() {
    xv::XVisio xVisio{};

    auto &devices = xVisio.getDevices();
    if (devices.empty()) {
        cout << "No devices found." << endl;
        exit(0);
    }
    const auto device = devices[0];
    cout << device->getUUID() << endl;
    cout << device->getVersion() << endl;

    auto slam = device->getSlam();
    slam->registerSlamCallback(&slamCallback);
    cout << "Timestamp, X, Y, Z" << endl;
    slam->start(xv::Slam::mode::Edge);
    std::signal(SIGINT, signal_handler);
    while (slam->running() && gSignalStatus != SIGINT) {

    }
    slam->stop();
}