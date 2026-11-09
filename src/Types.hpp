#pragma once
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using BodyableHandler = std::function<const char *(const httplib::Request &, httplib::Response &, json &)>;

class SSEWriter {
public:
    httplib::DataSink *sink;

    SSEWriter(httplib::DataSink *sink) : sink(sink) {}

    void write(json data) {
        write(data.dump());
    }

    void write(const char* data) {
        write(std::string(data));
    }

    void write(const std::string& data) {
        std::string toWrite = "data: " + data + "\n\n";
        sink->write(toWrite.data(), toWrite.size());
    }

    void finish() {
        sink->done();
    }
};

using StreamHandler = std::function<const char *(const httplib::Request *, httplib::Response *, bool, SSEWriter *)>;
using BodyableStreamHandler = std::function<const char *(const httplib::Request *, httplib::Response *, json &, bool, SSEWriter *)>;

enum class JsonType : uint8_t
{
    T_NULL,    ///< null value
    T_OBJECT,  ///< object (unordered set of name/value pairs)
    T_ARRAY,   ///< array (ordered collection of values)
    T_STRING,  ///< string value
    T_BOOL,    ///< boolean value
    T_INT,     ///< number value (signed integer)
    T_UINT,    ///< number value (unsigned integer)
    T_FLOAT,   ///< number value (floating-point)
    T_DOUBLE   ///< number value (floating-point)
};