//
// Created by Mihir Patil on 10/8/23.
//


#ifndef XVISIO_SLAM_H
#define XVISIO_SLAM_H

#include <vector>
#include "libusb.h"
#include "pose.h"
#include <functional>
#include <atomic>
#include <thread>

namespace xv {
    using slamCallback = std::function<void (Pose)>;

    class Device;

    class Slam {
    public:
        Slam(Device* pDevice, libusb_context* libusbContext, libusb_device_handle* libusbDeviceHandle);

        ~Slam();

        // copy constructor
        Slam(const Slam&other) = delete;

        // copy assignment
        Slam& operator=(const Slam&other)= delete;

        // move constructor
        Slam(Slam&&other) = delete;


        enum class mode: uint8_t {
            Edge = 0,
            Mixed
        };

        void start(mode mode);

        bool running() const;

        void stop();

        void registerSlamCallback(const std::function<void(Pose pose)>&callback);

    private:
        void slamHandler();

        LIBUSB_CALL static void usbCallback(libusb_transfer* transfer);

        std::vector<slamCallback> callbacks;
        Device* device;
        libusb_device_handle* handle;
        std::thread slamThread;
        std::atomic_bool runThread;
        libusb_context* context;
    };
} // xv

#endif //XVISIO_SLAM_H
