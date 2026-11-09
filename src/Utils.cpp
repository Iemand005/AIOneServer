#include "Utils.hpp"
#include <string>
#include <PreferredDevice.h>

PreferredDevice getPreferredDevice(std::string deviceStr) {
    if (deviceStr == "cpu") {
        return PreferredDevice::CPU;
    } else if (deviceStr == "igpu" || deviceStr == "integrated-gpu") {
        return PreferredDevice::IGPU;
    } else if (deviceStr == "dgpu" || deviceStr == "dedicated-gpu") {
        return PreferredDevice::DGPU;
    } else if (deviceStr == "accel" || deviceStr == "accelerator") {
        return PreferredDevice::ACCELERATOR;
    } else {
        return PreferredDevice::ANY;
    }
}