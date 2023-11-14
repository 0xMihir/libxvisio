//
// Created by Mihir Patil on 8/13/23.
//

#include "hid.h"
#include <string>

namespace xv {
    HID::HID(libusb_device_handle* handle) : handle(handle) {
    }

    void HID::write(std::array<uint8_t, 64>& data, const uint32_t timeout) const {
        if (const int res = libusb_control_transfer(
            handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, SET_REPORT, 0x0202, 3,
            data.data(), reqLen,
            timeout); res < LIBUSB_SUCCESS) {
            throw std::runtime_error(std::string("libusb error during write: ") + libusb_strerror(res));
        }
    }
}
