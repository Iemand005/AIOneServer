#include "RoutesLLM.hpp"
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "Types.hpp"
#include "AIOneServer.hpp"
#include "Utils.hpp"

void registerRoutesLLM(AIOneServer &server) {
    server.POST("/aio/llm/load", [&server](const httplib::Request *req, httplib::Response *res, json &body, bool stream, SSEWriter *writer) -> const char * {
        if (!server.mustHave(body, "path", JsonType::T_String)) return "path";

        std::string path = server.kString(body, "path");
        bool run = (!stream || writer != nullptr);


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
                        {"progress", progress}
                    };
                    writer->write(packet.dump());
                };
            }

            server.bundle.llms.push_back(server.modelFactory.loadLLM(path, options));

            json jsonRes;
            jsonRes["success"] = true;

            if (!stream) {
                res->set_content(jsonRes.dump(), "application/json");
            } else {
                writer->write(jsonRes.dump());
                writer->finish();
            }
        }

        return nullptr;
    });
}