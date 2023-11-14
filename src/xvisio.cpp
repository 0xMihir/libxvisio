//
// Created by Mihir Patil on 8/6/23.
//

#include <iostream>
#include "xvisio.h"

namespace xv {
    XVisio::XVisio() {
        if (const int res = libusb_init(&usb_ctx); res != 0) {
            throw std::runtime_error("USBInitFailure");
        }

        libusb_device **list = nullptr;
        const ssize_t nDevices = libusb_get_device_list(usb_ctx, &list);
        for (int i = 0; i < nDevices; ++i) {
            libusb_device_descriptor desc = {};
            libusb_get_device_descriptor(list[i], &desc);
            if (desc.idVendor == 0x040e && desc.idProduct == 0xf408) {
                auto devPtr = std::make_shared<Device>(list[i], usb_ctx);
                this->devices.push_back(devPtr);
            }
        }

        libusb_hotplug_register_callback(usb_ctx, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, LIBUSB_HOTPLUG_NO_FLAGS, 0x040e,
                                         0xf408, LIBUSB_HOTPLUG_MATCH_ANY,
                                         reinterpret_cast<libusb_hotplug_callback_fn>(&XVisio::hotPlugCallback),
                                         &devices, nullptr);
    }

    XVisio::XVisio(uint32_t timeout) {

    }

    XVisio::~XVisio() {
        devices.clear();
        libusb_exit(usb_ctx);
    }

    int
    XVisio::hotPlugCallback(libusb_context *ctx, libusb_device *device, libusb_hotplug_event,
                            std::vector<std::shared_ptr<Device>> *devices) {
        libusb_device_descriptor desc = {};
        libusb_get_device_descriptor(device, &desc);
        if (desc.idVendor == 0x040e && desc.idProduct == 0xf408) {
            const auto devPtr = std::make_shared<Device>(device, ctx);
            devices->push_back(devPtr);
        }
        return LIBUSB_SUCCESS;
    }

    const std::vector<std::shared_ptr<Device>> &XVisio::getDevices() {
        return devices;
    }

} // xv