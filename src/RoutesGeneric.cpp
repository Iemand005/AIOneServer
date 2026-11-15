#include "RoutesGeneric.hpp"
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "Version.hpp"
#include "Types.hpp"
#include "AIOneServer.hpp"
#include "ModelsBundle.hpp"
#include "Utils.hpp"

void registerRoutesGeneric(AIOneServer &server) {
    server.GET("/aio/info", [&server](const httplib::Request &req, httplib::Response &res) -> const char * {
        json jsonRes = {
            {"version", AIONE_SERVER_VERSION}
        };

        res.set_content(jsonRes.dump(), "application/json");
        return nullptr;
    });
    
    server.GET("/aio/models", [&server](const httplib::Request &req, httplib::Response &res) -> const char * {
        json jsonRes = json::array();

        for (const auto &[id, model] : server.bundle.models) {
            json modelJson = {
                {"id", model.id},
                {"type", getHostableModelType(model.type)},
                {"loaded", true}
            };

            jsonRes.push_back(modelJson);
        }
        
        for (const auto &[id, entry] : server.bundle.registeredIds) {
            if (hasKey(server.bundle.models, id)) {
                // model is already loaded
                continue;
            }

            json modelJson = {
                {"id", entry.id},
                {"type", getHostableModelType(entry.type)},
                {"loaded", false}
            };

            jsonRes.push_back(modelJson);
        }

        res.set_content(jsonRes.dump(), "application/json");
        return nullptr;
    });
}