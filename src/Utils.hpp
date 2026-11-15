#pragma once
#include <string>
#include <unordered_map>
#include <PreferredDevice.h>

#include "ModelsBundle.hpp"

PreferredDevice getPreferredDevice(std::string deviceStr);

std::string getHostableModelType(HostableModelType type);

template <typename... Args>
bool hasKey(const std::unordered_map<Args...> &map, const std::string &key) {
    return map.find(key) != map.end();
}

template <typename... Args>
bool hasKey(const std::vector<Args...>& vec, const std::string& key) {
    return std::find(vec.begin(), vec.end(), key) != vec.end();
}