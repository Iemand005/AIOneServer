#pragma once
#include <vector>

#include <LLModel.hpp>
#include <SDModel.hpp>
#include <TTSModel.hpp>

enum class HostableModelType {
    Unknown,
    LLM,
    SD,
    TTS,
};

struct HostableModel {
    std::string id = "";
    HostableModelType type = HostableModelType::Unknown;
    LLModelPtr llm = nullptr;
    SDModelPtr sd = nullptr;
    TTSModelPtr tts = nullptr;
    
    HostableModel() = default;                                // allow init
    HostableModel(const HostableModel &) = delete;            // don't allow copy
    HostableModel& operator=(const HostableModel &) = delete; // don't allow copy
    HostableModel(HostableModel&&) = default;                 // allow move
    HostableModel& operator=(HostableModel&&) = default;      // allow move

    void free() {
        // TODO await lock or smth?
        
        if ((type == HostableModelType::LLM || type == HostableModelType::Unknown) && llm != nullptr) {
            llm = nullptr;
        } else if ((type == HostableModelType::SD || type == HostableModelType::Unknown) && sd != nullptr) {
            sd = nullptr;
        } else if ((type == HostableModelType::TTS || type == HostableModelType::Unknown) && tts != nullptr) {
            tts = nullptr;
        }
    }
};

class ModelsBundle {
public:
    std::unordered_map<std::string, HostableModel> models;
    
    HostableModel *find(std::string id) {
        auto it = models.find(id);
        if (it != models.end()) {
            return &it->second;
        }

        return nullptr;
    }
};