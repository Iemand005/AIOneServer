#pragma once
#include <vector>

#include <LLModel.hpp>
#include <SDModel.hpp>
#include <TTSModel.hpp>

class ModelsBundle {
public:
    std::vector<LLModelPtr> llms;
    std::vector<SDModelPtr> sdms;
    std::vector<TTSModelPtr> ttsms;
};