//
// Created by Mihir Patil on 10/8/23.
//

#include "slam.h"
#include "device.h"
#include <array>
#include <iostream>
#include <chrono>

constexpr double float_scale = 6.103515625e-05;

namespace xv {
    Slam::Slam(Device* pDevice, libusb_context* libusbContext,
               libusb_device_handle* libusbDeviceHandle) : device(pDevice), handle(libusbDeviceHandle),
                                                           context(libusbContext) {
    }

    Slam::~Slam() {
        runThread = false;
        if (slamThread.joinable()) {
            slamThread.join();
        }
    }

    void Slam::stop() {
        runThread = false;
        if (slamThread.joinable()) {
            slamThread.join();
        }
    }

    void Slam::registerSlamCallback(const std::function<void(Pose)>&callback) {
        callbacks.push_back(callback);
    }

    void Slam::start(const mode mode) {
        device->configureDevice(mode == mode::Edge, 1, mode != mode::Edge);
        device->startEdgeStream(1, false, false);
        runThread = true;
        slamThread = std::thread(&Slam::slamHandler, this);
    }

    bool Slam::running() const {
        return runThread;
    }

    void Slam::slamHandler() {
        libusb_transfer* transfer = libusb_alloc_transfer(0);
        const auto buffer = std::make_shared<std::array<uint8_t, 64>>();
        libusb_fill_interrupt_transfer(transfer, handle, 0x83, buffer->data(), 63,
                                       &Slam::usbCallback, &callbacks,
                                       5000);
        if (const int result = libusb_submit_transfer(transfer); result != LIBUSB_SUCCESS) {
            std::cerr << libusb_strerror(result) << std::endl;
            runThread = false;
            return;
        }

        while (runThread) {
            libusb_handle_events(context);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        libusb_cancel_transfer(transfer);
    }

    void Slam::usbCallback(libusb_transfer* transfer) {
        if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
            libusb_submit_transfer(transfer);


            auto* buffer = transfer->buffer;
            const auto timestamp = *reinterpret_cast<uint32_t *>(&buffer[3]);
            const auto* translationInts = reinterpret_cast<int32_t *>(&buffer[7]);
            const auto* rotationMatrix = reinterpret_cast<int16_t *>(&buffer[19]);

            const Vector3 translation = {
                translationInts[0] * float_scale, translationInts[1] * float_scale, translationInts[2] * float_scale
            };
            const Matrix3 matrix = {
                {
                    {rotationMatrix[0] * float_scale, rotationMatrix[1] * float_scale, rotationMatrix[2] * float_scale},
                    {rotationMatrix[3] * float_scale, rotationMatrix[4] * float_scale, rotationMatrix[5] * float_scale},
                    {rotationMatrix[6] * float_scale, rotationMatrix[7] * float_scale, rotationMatrix[8] * float_scale}
                }
            };

            const Pose pose{translation, matrix, timestamp};

            for (const auto callbacks = *static_cast<std::vector<slamCallback> *>(transfer->user_data); const auto&callback:
                 callbacks) {
                callback(pose);
                 }
        }
    }
} // xv
