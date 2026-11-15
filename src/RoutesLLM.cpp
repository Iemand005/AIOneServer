#include "RoutesLLM.hpp"
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "Types.hpp"
#include "AIOneServer.hpp"
#include "ModelsBundle.hpp"
#include "Utils.hpp"

void registerRoutesLLM(AIOneServer &server) {
    server.POST("/aio/llm/load", [&server](const httplib::Request *req, httplib::Response *res, json &body, bool stream, SSEWriter *writer) -> const char * {
        bool run = (!stream || writer != nullptr);

        if (!server.mustHave(body, "id", JsonType::T_FilledString)) return "id";
        if (!server.mustHave(body, "path", JsonType::T_FilledString)) return "path";

        std::string modelId = server.kString(body, "id");
        std::string path = server.kString(body, "path");

        if (server.bundle.models.find(modelId) != server.bundle.models.end()) {
            throw std::runtime_error("model with this id is already loaded");
        }


        if (run) {
            LLModelOptions options;
            SET_PROP_TO_JSON_VAL(body, Int, options, offloadLayers);
            SET_PROP_TO_JSON_VAL(body, Bool, options, useMmap);
            SET_PROP_TO_JSON_VAL(body, Bool, options, flashAttention);
            SET_PROP_TO_JSON_VAL(body, Uint, options, contextLength);
            SET_PROP_TO_JSON_VAL(body, Uint, options, evalBatchSize);

            if (server.mustHave(body, "device", JsonType::T_String)) {
                options.device = getPreferredDevice(server.kString(body, "device"));
            }

            if (stream) {
                options.onProgress = [&writer](const float progress, const int step, const int steps) {
                    json packet = {
                        {"type", "progress"},
                        {"progress", progress}
                    };
                    writer->write(packet.dump());
                };
            }


            HostableModel model = {modelId, HostableModelType::LLM};
            model.llm = server.modelFactory.loadLLM(path, options);
            server.bundle.models[modelId] = std::move(model);

            json jsonRes;
            jsonRes["success"] = true;

            if (!stream) {
                res->set_content(jsonRes.dump(), "application/json");
            } else {
                jsonRes["type"] = "done";
                writer->write(jsonRes.dump());
                writer->finish();
            }
        }

        return nullptr;
    });

    server.POST("/aio/llm/unload", [&server](const httplib::Request &req, httplib::Response &res, json &body) -> const char * {
        if (!server.mustHave(body, "id", JsonType::T_FilledString)) return "id";

        std::string modelId = server.kString(body, "id");
        
        HostableModel *model = server.bundle.find(modelId);
        if (model == nullptr) {
            throw std::runtime_error("model with this id is not loaded");
        }

        model->free();
        server.bundle.models.erase(modelId);

        json jsonRes;
        jsonRes["success"] = true;
        res.set_content(jsonRes.dump(), "application/json");
        return nullptr;
    });
}