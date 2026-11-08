#include "httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    httplib::Server svr;

    svr.Post("/api/hello", [](const httplib::Request& req, httplib::Response& res) {
        json req_json = json::parse(req.body, nullptr, false);
        
        json res_json;
        res_json["message"] = "Hello " + req_json.value("name", "User");
        
        res.set_content(res_json.dump(), "application/json");
    });

    svr.listen("0.0.0.0", 8080);
}