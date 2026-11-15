#include "RoutesLLM.hpp"
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "Types.hpp"
#include "AIOneServer.hpp"
#include "ModelsBundle.hpp"
#include "Utils.hpp"

void createContext(HostableModel *model, json *ctxIds) {
    const std::string ctxId = uuidv4();
    RegisteredContext ctx = {
        /* .id = */ ctxId
    };

    ctx.modelCtx = model->llm->createContext();

    if (ctxIds != nullptr) {
        ctxIds->push_back(ctxId);
    }
}

void registerRoutesLLM(AIOneServer &server) {
    server.POST("/aio/llm/register", [&server](const httplib::Request &req, httplib::Response &res, json &body) -> const char * {
        if (!server.mustHave(body, "modelId", JsonType::T_FilledString)) return "modelId";
        if (!server.mustHave(body, "path", JsonType::T_FilledString)) return "path";

        std::string modelId = server.kString(body, "modelId");
        std::string path = server.kString(body, "path");

        if (hasKey(server.bundle.registeredIds, modelId)) {
            throw std::runtime_error("Model with this id is already registered");
        }
        
        RegisteredModel entry = {
            /* .id   = */ modelId,
            /* .path = */ path,
            /* .type = */ HostableModelType::LLM
        };
        server.bundle.registeredIds[modelId] = entry;

        json jsonRes;
        jsonRes["success"] = true;
        res.set_content(jsonRes.dump(), "application/json");
        return nullptr;
    });

    server.POST("/aio/llm/load", [&server](const httplib::Request *req, httplib::Response *res, json &body, bool stream, SSEWriter *writer) -> const char * {
        bool run = (!stream || writer != nullptr);

        if (!server.mustHave(body, "modelId", JsonType::T_FilledString)) return "modelId";

        std::string modelId = server.kString(body, "modelId");
        if (hasKey(server.bundle.models, modelId)) {
            throw std::runtime_error("Model with this id is already loaded");
        }

        std::string path;
        
        if (server.mustHave(body, "path", JsonType::T_FilledString)) {
            path = server.kString(body, "path");
        } else {
            // find registered model
            if (!hasKey(server.bundle.registeredIds, modelId)) {
                throw std::runtime_error("Model with this id was not registered and no path was given");
            }

            const RegisteredModel &entry = server.bundle.registeredIds[modelId];
            if (entry.type != HostableModelType::LLM) {
                throw std::runtime_error("Registered model with this id is not an llm");
            }

            path = entry.path;
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


            HostableModel model = {
                /* .id   = */ modelId,
                /* .type = */ HostableModelType::LLM
            };
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
        if (!server.mustHave(body, "modelId", JsonType::T_FilledString)) return "modelId";

        std::string modelId = server.kString(body, "modelId");
        HostableModel *model = server.bundle.findModel(modelId, HostableModelType::LLM);
        if (model == nullptr) {
            throw std::runtime_error("Model with this id is not loaded");
        }

        model->free();
        server.bundle.models.erase(modelId);

        json jsonRes;
        jsonRes["success"] = true;
        res.set_content(jsonRes.dump(), "application/json");
        return nullptr;
    });

    server.POST("/aio/llm/context", [&server](const httplib::Request &req, httplib::Response &res, json &body) -> const char * {
        if (!server.mustHave(body, "modelId", JsonType::T_FilledString)) return "modelId";

        std::string modelId = server.kString(body, "modelId");
        HostableModel *model = server.bundle.findModel(modelId, HostableModelType::LLM);
        if (model == nullptr) {
            throw std::runtime_error("Model with this id is not loaded");
        }
        
        unsigned int amount = 1;
        if (server.mustHave(body, "amount", JsonType::T_Uint)) {
            amount = server.kUint(body, "amount");
        }
        
        json ctxIds = json::array();
        for (size_t i = 0; i < amount; i++) {
            createContext(model, &ctxIds);
        }

        json jsonRes;
        jsonRes["success"] = true;
        jsonRes["ctxIds"] = ctxIds;
        res.set_content(jsonRes.dump(), "application/json");
        return nullptr;
    });

    // TODO DELETE /aio/llm/context -> gotta `model->llm->unregisterContext(ctx)` or else memory leak
    
    server.POST("/aio/llm/complete", [&server](const httplib::Request *req, httplib::Response *res, json &body, bool stream, SSEWriter *writer) -> const char * {
        bool run = (!stream || writer != nullptr);

        if (!server.mustHave(body, "modelId", JsonType::T_FilledString)) return "modelId";
        if (!server.mustHave(body, "prompt", JsonType::T_String)) return "prompt";

        std::string modelId = server.kString(body, "modelId");
        HostableModel *model = server.bundle.findModel(modelId, HostableModelType::LLM);
        if (model == nullptr) {
            throw std::runtime_error("Model with this id is not loaded");
        }

        std::string prompt = server.kString(body, "prompt");

        // TODO ctxId
        

        if (run) {
            TextGenOptions options;
            SET_PROP_TO_JSON_VAL(body, Int, options, maxTokens);
            SET_PROP_TO_JSON_VAL(body, Float, options, minP);
            SET_PROP_TO_JSON_VAL(body, Int, options, topK);
            SET_PROP_TO_JSON_VAL(body, Float, options, temperature);
            SET_PROP_TO_JSON_VAL(body, Uint, options, seed);

            if (stream) {
                std::string thinkStart = "<think>";
                if (server.mustHave(body, "customThinkStart", JsonType::T_String)) {
                    thinkStart = server.kString(body, "customThinkStart");
                }

                std::string thinkStop = "</think>";
                if (server.mustHave(body, "customThinkStop", JsonType::T_String)) {
                    thinkStop = server.kString(body, "customThinkStop");
                }

                options.onInputEval = [&writer](const float progress, const int step, const int steps) {
                    json packet = {
                        {"type", "progress"},
                        {"progress", progress}
                    };
                    writer->write(packet.dump());
                };

                options.onGenerationStart = [&writer]() {
                    json packet = {
                        {"type", "start"}
                    };
                    writer->write(packet.dump());
                };

                options.onThinkStateChange = [&writer, thinkStart, thinkStop](bool thinking) {
                    json packet = {
                        {"type", "token"},
                        {"token", thinking ? thinkStart : thinkStop},
                        {"thinking", thinking}
                    };
                    writer->write(packet.dump());
                };
                
                options.onToken = [&writer](const std::string &token) {
                    json packet = {
                        {"type", "token"},
                        {"token", token}
                    };
                    writer->write(packet.dump());
                };
            }

            if (model->contexts.empty()) {
                createContext(model, nullptr);
            }
            
            TextGenResult result = model->llm->completeAny(prompt, options);

            json jsonRes;
            jsonRes["success"] = true;
            jsonRes["text"] = result.output.content;

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
}