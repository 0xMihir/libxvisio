//
// Created by Mihir Patil on 8/13/23.
//

#ifndef XVISIO_HID_IMPL_H
#define XVISIO_HID_IMPL_H

#include <array>
#include <string>
#include <algorithm>

namespace xv {
    template<size_t cmdLen>
    bool
    HID::executeTransaction(std::array<uint8_t, cmdLen>&command, std::array<uint8_t, 62 - cmdLen>&data,
                            const uint32_t timeout) {
        static_assert(cmdLen <= reqLen - 1, "HID command cannot be longer than 62 bytes");
        constexpr uint8_t requestType =
                LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE; // NOLINT(*-suspicious-enum-usage)

        std::array<uint8_t, reqLen> sendData = {0x02}; // 1 byte for direction, 62 bytes for command
        std::copy(command.begin(), command.end(), sendData.begin() + 1);


        if (const int res = libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | requestType, SET_REPORT, 0x0202, 3,
                                                    sendData.data(), reqLen,
                                                    timeout); res < LIBUSB_SUCCESS) {
            throw std::runtime_error(std::string("libusb error during write: ") + libusb_strerror(res));
        }

        std::array<uint8_t, reqLen> receiveData = {0}; // 1 byte for direction, 62 bytes for command

        if (const int res = libusb_control_transfer(handle, LIBUSB_ENDPOINT_IN | requestType, GET_REPORT, 0x0101, 3,
                                                    receiveData.data(), reqLen, timeout); res < LIBUSB_SUCCESS) {
            throw std::runtime_error(std::string("libusb error during read: ") + libusb_strerror(res));
        }

        if (receiveData[0] != 0x01) {
            return false;
        }

        for (int i = 1; i < cmdLen; ++i) {
            if (sendData[i] != receiveData[i]) {
                return false;
            }
        }

        std::copy(receiveData.begin() + cmdLen + 1, receiveData.end(), data.begin());
        return true;
    }
}
#endif //XVISIO_HID_IMPL_H
