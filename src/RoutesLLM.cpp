#pragma once
#include "RoutesLLM.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "Types.hpp"
#include "AIOneServer.hpp"

void registerRoutesLLM(AIOneServer &server) {
    server.POST("/aio/llm/load", [&server](const httplib::Request *req, httplib::Response *res, json &body, bool stream, SSEWriter *writer) -> const char * {
        if (!server.mustHave(body, "model", JsonType::T_STRING)) return "model";

        std::string model = server.kString(body, "model");


        if (!stream) {
            json jsonRes;
            jsonRes["success"] = true;
            res->set_content(jsonRes.dump(), "application/json");
        } else if (writer != nullptr) {
            writer->write("Sup bro");
            writer->finish();
        }

        return nullptr;
    });
}