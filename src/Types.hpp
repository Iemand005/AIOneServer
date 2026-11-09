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

    bool write(const std::string& data) {
        std::string toWrite = "data: " + data + "\n\n";

        if (sink->is_writable()) {
            sink->write(toWrite.data(), toWrite.size());
            return true;
        }

        return false;
    }

    void finish() {
        sink->done();
    }
};

using StreamHandler = std::function<const char *(const httplib::Request *, httplib::Response *, bool, SSEWriter *)>;
using BodyableStreamHandler = std::function<const char *(const httplib::Request *, httplib::Response *, json &, bool, SSEWriter *)>;

enum class JsonType : uint8_t
{
    T_Null,    ///< null value
    T_Object,  ///< object (unordered set of name/value pairs)
    T_Array,   ///< array (ordered collection of values)
    T_String,  ///< string value
    T_Bool,    ///< boolean value
    T_Int,     ///< number value (signed integer)
    T_Uint,    ///< number value (unsigned integer)
    T_Float,   ///< number value (floating-point)
    T_Double   ///< number value (floating-point)
};