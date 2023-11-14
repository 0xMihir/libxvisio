#ifndef LIBXVISIO_DEVICE_H
#define LIBXVISIO_DEVICE_H

#include <memory>
#include <string>
#include "libusb.h"
#include "hid.h"

namespace xv {
    class Slam;

    class Device {
    public:
        explicit Device(libusb_device* libusbDevice, libusb_context* context);

        ~Device();

        std::string& getUUID();

        std::string& getVersion();


        [[nodiscard]] bool getEdgeModeSupport() const;

        [[nodiscard]] bool get_mixed_mode_support() const;

        [[nodiscard]] bool getStereoSupport() const;

        [[nodiscard]] bool getRGBSupport() const;

        [[nodiscard]] bool getToFSupport() const;

        [[nodiscard]] bool getIASupport() const;

        [[nodiscard]] bool getSGBMSupport() const;


        [[nodiscard]] bool getEyeTrackingSupport() const;

        [[nodiscard]] bool getFaceIDSupport() const;

        [[nodiscard]] std::shared_ptr<Slam> getSlam() const;

        void configureDevice(bool edge6dof, uint8_t uvcMode, bool embeddedAlgo) const;

        void startEdgeStream(uint8_t edgeMode, bool rotationEnabled, bool flipped) const;

    private:
        libusb_device* const libusbDevice;
        libusb_context* libusbContext;
        libusb_device_handle* handle;
        std::unique_ptr<HID> hid;
        std::string uuid;
        std::string version;
        uint32_t featuresBitmap;
        std::array<float, 14> imuBias;
        std::shared_ptr<Slam> slam;
    };
}

#endif //LIBXVISIO_DEVICE_H
