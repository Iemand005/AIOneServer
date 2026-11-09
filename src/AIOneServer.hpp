#pragma once
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "Types.hpp"

class AIOneServer {
public:

    httplib::Server *server;

    AIOneServer(httplib::Server *server) : server(server) {}

    httplib::Server * getServer() const {
        return server;
    }

    bool mustHave(json &obj, const char *key) const {
        return obj.contains(key);
    }

    bool mustHave(json &obj, const char *key, JsonType type) const {
        if (!obj.contains(key)) {
            return false;
        }

        json val = obj[key];

        if (type == JsonType::T_NULL) {
            return val.is_null();
        } else if (type == JsonType::T_OBJECT) {
            return val.is_object();
        } else if (type == JsonType::T_ARRAY) {
            return val.is_array();
        } else if (type == JsonType::T_STRING) {
            return val.is_string();
        } else if (type == JsonType::T_BOOL) {
            return val.is_boolean();
        } else if (type == JsonType::T_INT) {
            return val.is_number() && !val.is_number_float();
        } else if (type == JsonType::T_UINT) {
            return val.is_number_unsigned();
        } else if (type == JsonType::T_FLOAT) {
            return val.is_number();
        }

        return false;
    }

    json kObject(json &obj, const char *key) const {
        return obj[key];
    }

    json kArray(json &obj, const char *key) const {
        return obj[key];
    }

    std::string kString(json &obj, const char *key) const {
        return obj[key].get<std::string>();
    }

    bool kBool(json &obj, const char *key) const {
        json val = obj[key];
        return val.is_number() ? val.get<double>() > 0 : val.get<bool>();
    }

    int kInt(json &obj, const char *key) const {
        return obj[key].get<int>();
    }

    unsigned int kUint(json &obj, const char *key) const {
        return obj[key].get<unsigned int>();
    }

    float kFloat(json &obj, const char *key) const {
        return obj[key].get<float>();
    }

    double kDouble(json &obj, const char *key) const {
        return obj[key].get<double>();
    }

    void GET(const char *path, httplib::Server::Handler handler) {
        // TODO parse query params
        // TODO streaming
        server->Get(path, handler);
    }
    
    void POST(const char *path, BodyableHandler handler) {
        POST(path, handler, nullptr);
    }
    
    void POST(const char *path, BodyableStreamHandler handler) {
        POST(path, nullptr, handler);
    }

private:
    void POST(const char *path, BodyableHandler handler, BodyableStreamHandler streamHandler) {
        server->Post(path, [this, handler, streamHandler](const httplib::Request& req, httplib::Response& res) {
            json body = json::parse(req.body, nullptr, false);

            bool isStreaming = false;
            if (streamHandler != nullptr) {
                if (mustHave(body, "stream", JsonType::T_BOOL)) {
                    isStreaming = kBool(body, "stream");
                }
            }

            const char *validation = (streamHandler != nullptr ? streamHandler(&req, &res, body, isStreaming, nullptr) : handler(req, res, body));

            if (validation != nullptr) {
                json jsonRes;
                jsonRes["error"] = "Invalid param: " + std::string(validation);
                res.status = 400;
                res.set_content(jsonRes.dump(), "application/json");
                return;
            }

            if (isStreaming) {
                res.set_chunked_content_provider("text/event-stream", [streamHandler = std::move(streamHandler), body = std::move(body)](size_t offset, httplib::DataSink &sink) mutable -> bool {
                    SSEWriter writer(&sink);
                    streamHandler(static_cast<const httplib::Request *>(nullptr), static_cast<httplib::Response *>(nullptr), body, true, &writer);
                    return true;
                });
            }
        });
    }
};