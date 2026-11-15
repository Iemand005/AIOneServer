#include "Utils.hpp"
#include <string>
#include <PreferredDevice.h>

#include "ModelsBundle.hpp"

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

std::string getHostableModelType(HostableModelType type) {
    if (type == HostableModelType::LLM) {
        return "llm";
    } else if (type == HostableModelType::SD) {
        return "sd";
    } else if (type == HostableModelType::TTS) {
        return "tts";
    } else {
        return "unknown";
    }
}

