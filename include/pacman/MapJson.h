//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     mapJson data = nlohmann::json::parse(jsonString);

#pragma once

#include "pacman/json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

namespace quicktype {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_quicktype_HELPER
    #define NLOHMANN_UNTYPED_quicktype_HELPER
    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }
    #endif

    class mapJson {
        public:
        mapJson() = default;
        virtual ~mapJson() = default;

        private:
        std::string name;
        int64_t width;
        int64_t height;
        std::vector<std::string> layout;

        public:
        const std::string & get_name() const { return name; }
        std::string & get_mutable_name() { return name; }
        void set_name(const std::string & value) { this->name = value; }

        const int & get_width() const { return width; }
        int64_t & get_mutable_width() { return width; }
        void set_width(const int64_t & value) { this->width = value; }

        const int & get_height() const { return height; }
        int64_t & get_mutable_height() { return height; }
        void set_height(const int64_t & value) { this->height = value; }

        const std::vector<std::string> & get_layout() const { return layout; }
        std::vector<std::string> & get_mutable_layout() { return layout; }
        void set_layout(const std::vector<std::string> & value) { this->layout = value; }
    };
}

namespace quicktype {
    void from_json(const json & j, mapJson & x);
    void to_json(json & j, const mapJson & x);

    inline void from_json(const json & j, mapJson& x) {
        x.set_name(j.at("name").get<std::string>());
        x.set_width(j.at("width").get<int64_t>());
        x.set_height(j.at("height").get<int64_t>());
        x.set_layout(j.at("layout").get<std::vector<std::string>>());
    }

    inline void to_json(json & j, const mapJson & x) {
        j = json::object();
        j["name"] = x.get_name();
        j["width"] = x.get_width();
        j["height"] = x.get_height();
        j["layout"] = x.get_layout();
    }
}
