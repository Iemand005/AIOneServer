#include <httplib.h>
#include <nlohmann/json.hpp>

#include "AIOneServer.hpp"
#include "RoutesGeneric.hpp"
#include "RoutesLLM.hpp"

using json = nlohmann::json;

int main() {
    httplib::Server httpServer;
    AIOneServer server(&httpServer);

    // /aio/info
    // /aio/models
    registerRoutesGeneric(server);
    // /aio/llm/load
    registerRoutesLLM(server);

    httpServer.listen("0.0.0.0", 8080);
}