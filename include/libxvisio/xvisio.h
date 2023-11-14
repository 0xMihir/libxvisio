//
// Created by Mihir Patil on 8/6/23.
//

#ifndef LIBXVISIO_XVISIO_H
#define LIBXVISIO_XVISIO_H

#include <vector>
#include <memory>
#include <libusb.h>
#include "device.h"
#include "slam.h"

namespace xv {
    class XVisio {
    public:
        XVisio();

        explicit XVisio(uint32_t timeout);

        ~XVisio();

        const std::vector<std::shared_ptr<Device>>& getDevices();

    private:
        libusb_context* usb_ctx = nullptr;
        std::vector<std::shared_ptr<Device>> devices;
        LIBUSB_CALL static int
        hotPlugCallback(libusb_context*, libusb_device* device, libusb_hotplug_event,
                        std::vector<std::shared_ptr<Device>>* devices);
    };
} // xv

#endif //LIBXVISIO_XVISIO_H
