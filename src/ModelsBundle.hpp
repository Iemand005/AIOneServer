#pragma once
#include <vector>
#include <memory>

#include <LLModel.hpp>
#include <SDModel.hpp>
#include <TTSModel.hpp>

enum class HostableModelType {
    Unknown,
    LLM,
    SD,
    TTS,
};

struct RegisteredContext {
    std::string id = "";
    std::shared_ptr<TextContext> modelCtx;
};

struct HostableModel {
    std::string id = "";
    HostableModelType type = HostableModelType::Unknown;
    std::unordered_map<std::string, RegisteredContext> contexts;

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

struct RegisteredModel {
    std::string id = "";
    std::string path = "";
    HostableModelType type = HostableModelType::Unknown;
};

class ModelsBundle {
public:
    std::unordered_map<std::string, HostableModel> models;
    std::unordered_map<std::string, RegisteredModel> registeredIds;
    
    HostableModel *findModel(const std::string &id) {
        auto it = models.find(id);
        if (it != models.end()) {
            return &it->second;
        }

        return nullptr;
    }

    HostableModel *findModel(const std::string &id, HostableModelType type) {
        HostableModel *model = findModel(id);
        if (model->type != type) {
            return nullptr;
        }

        return model;
    }
};