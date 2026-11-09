#include <httplib.h>
#include <nlohmann/json.hpp>

#include "AIOneServer.hpp"
#include "RoutesLLM.hpp"

using json = nlohmann::json;

int main() {
    httplib::Server httpServer;
    AIOneServer server(&httpServer);

    // /aione/info
    // /aione/models
    registerRoutesLLM(server);

    httpServer.listen("0.0.0.0", 8080);
}