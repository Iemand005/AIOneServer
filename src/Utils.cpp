#include "Utils.hpp"
#include <string>
#include <random>
#include <sstream>
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

std::string uuidv4() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis;

    uint64_t part1 = dis(gen);
    uint64_t part2 = dis(gen);

    part1 = (part1 & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
    part2 = (part2 & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;

    std::stringstream ss;
    ss << std::hex << std::nouppercase;

    auto format_hex = [&](uint64_t val, int width) {
        ss.width(width);
        ss.fill('0');
        ss << val;
    };

    format_hex(part1 >> 32, 8);
    ss << "-";
    format_hex((part1 >> 16) & 0xFFFF, 4);
    ss << "-";
    format_hex(part1 & 0xFFFF, 4);
    ss << "-";
    format_hex(part2 >> 48, 4);
    ss << "-";
    format_hex(part2 & 0xFFFFFFFFFFFFULL, 12);

    return ss.str();
}