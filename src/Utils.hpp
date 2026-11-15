#pragma once
#include <string>
#include <PreferredDevice.h>

#include "ModelsBundle.hpp"

PreferredDevice getPreferredDevice(std::string deviceStr);

std::string getHostableModelType(HostableModelType type);