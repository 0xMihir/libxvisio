//
// Created by Mihir Patil on 8/13/23.
//

#ifndef LIBXVISIO_HID_H
#define LIBXVISIO_HID_H

#include <libusb.h>
#include <array>

#define SET_REPORT 0x09
#define GET_REPORT 0x01

namespace xv {
    class HID {
    public:
        static constexpr uint8_t reqLen = 63;

        explicit HID(libusb_device_handle *handle);

        template<size_t cmdLen>
        bool executeTransaction(std::array<uint8_t, cmdLen> &command, std::array<uint8_t, 62 - cmdLen> &data,
                                uint32_t timeout = 1000);

        void write(std::array<uint8_t, 64>& data, uint32_t timeout = 1000) const;

    private:
        libusb_device_handle *handle;

    };
}

#include "hid_impl.h"

#endif //LIBXVISIO_HID_H

