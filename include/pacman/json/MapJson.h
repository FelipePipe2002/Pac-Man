#pragma once

#include "json.hpp"

#include <optional>
#include <regex>
#include <stdexcept>

namespace quicktype
{
    using nlohmann::json;

#ifndef NLOHMANN_UNTYPED_quicktype_HELPER
#define NLOHMANN_UNTYPED_quicktype_HELPER
    inline json get_untyped(const json& j, const char* property)
    {
        if (j.find(property) != j.end())
        {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(json const& j, std::string property)
    {
        return get_untyped(j, property.data());
    }
#endif

    class ghost
    {
    public:
        ghost() = default;
        virtual ~ghost() = default;

    private:
        int64_t id;
        std::string color;
        std::vector<int64_t> pos;
        std::string scatter;
        std::string mode;

    public:
        int64_t const& get_id() const
        {
            return id;
        }
        int64_t& get_mutable_id()
        {
            return id;
        }
        void set_id(int64_t const& value)
        {
            this->id = value;
        }

        std::string const& get_color() const
        {
            return color;
        }
        std::string& get_mutable_color()
        {
            return color;
        }
        void set_color(std::string const& value)
        {
            this->color = value;
        }

        std::vector<int64_t> const& get_pos() const
        {
            return pos;
        }
        std::vector<int64_t>& get_mutable_pos()
        {
            return pos;
        }
        void set_pos(std::vector<int64_t> const& value)
        {
            this->pos = value;
        }

        std::string const& get_scatter() const
        {
            return scatter;
        }
        std::string& get_mutable_scatter()
        {
            return scatter;
        }
        void set_scatter(std::string const& value)
        {
            this->scatter = value;
        }

        std::string const& get_mode() const
        {
            return mode;
        }
        std::string& get_mutable_mode()
        {
            return mode;
        }
        void set_mode(std::string const& value)
        {
            this->mode = value;
        }
    };

    class portals
    {
    public:
        portals() = default;
        virtual ~portals() = default;

    private:
        std::map<std::string, std::vector<std::vector<int64_t>>> data;

    public:
        std::map<std::string, std::vector<std::vector<int64_t>>> const& get_data() const
        {
            return data;
        }
        std::map<std::string, std::vector<std::vector<int64_t>>>& get_mutable_data()
        {
            return data;
        }
        void set_data(std::map<std::string, std::vector<std::vector<int64_t>>> const& value)
        {
            data = value;
        }
    };

    class mapJson
    {
    public:
        mapJson() = default;
        virtual ~mapJson() = default;

    private:
        std::string name;
        int64_t width;
        int64_t height;
        std::vector<std::string> layout;
        std::map<std::string, std::vector<int64_t>> scatter_points;
        portals portals;
        std::vector<ghost> ghosts;

    public:
        std::string const& get_name() const
        {
            return name;
        }
        std::string& get_mutable_name()
        {
            return name;
        }
        void set_name(std::string const& value)
        {
            this->name = value;
        }

        int64_t const& get_width() const
        {
            return width;
        }
        int64_t& get_mutable_width()
        {
            return width;
        }
        void set_width(int64_t const& value)
        {
            this->width = value;
        }

        int64_t const& get_height() const
        {
            return height;
        }
        int64_t& get_mutable_height()
        {
            return height;
        }
        void set_height(int64_t const& value)
        {
            this->height = value;
        }

        std::vector<std::string> const& get_layout() const
        {
            return layout;
        }
        std::vector<std::string>& get_mutable_layout()
        {
            return layout;
        }
        void set_layout(std::vector<std::string> const& value)
        {
            this->layout = value;
        }

        std::map<std::string, std::vector<int64_t>> const& get_scatter_points() const
        {
            return scatter_points;
        }
        std::map<std::string, std::vector<int64_t>>& get_mutable_scatter_points()
        {
            return scatter_points;
        }
        void set_scatter_points(std::map<std::string, std::vector<int64_t>> const& value)
        {
            this->scatter_points = value;
        }

        quicktype::portals const& get_portals() const
        {
            return portals;
        }
        quicktype::portals& get_mutable_portals()
        {
            return portals;
        }
        void set_portals(quicktype::portals const& value)
        {
            this->portals = value;
        }

        std::vector<ghost> const& get_ghosts() const
        {
            return ghosts;
        }
        std::vector<ghost>& get_mutable_ghosts()
        {
            return ghosts;
        }
        void set_ghosts(std::vector<ghost> const& value)
        {
            this->ghosts = value;
        }
    };
} // namespace quicktype

namespace quicktype
{
    void from_json(json const& j, ghost& x);
    void to_json(json& j, ghost const& x);

    void from_json(json const& j, portals& x);
    void to_json(json& j, portals const& x);

    void from_json(json const& j, mapJson& x);
    void to_json(json& j, mapJson const& x);

    inline void from_json(json const& j, ghost& x)
    {
        x.set_id(j.at("id").get<int64_t>());
        x.set_color(j.at("color").get<std::string>());
        x.set_pos(j.at("pos").get<std::vector<int64_t>>());
        x.set_scatter(j.at("scatter").get<std::string>());
        x.set_mode(j.at("mode").get<std::string>());
    }

    inline void to_json(json& j, ghost const& x)
    {
        j = json::object();
        j["id"] = x.get_id();
        j["color"] = x.get_color();
        j["pos"] = x.get_pos();
        j["scatter"] = x.get_scatter();
        j["mode"] = x.get_mode();
    }

    inline void from_json(json const& j, portals& x)
    {
        std::map<std::string, std::vector<std::vector<int64_t>>> temp;
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            temp[it.key()] = it.value().get<std::vector<std::vector<int64_t>>>();
        }
        x.set_data(temp);
    }

    inline void to_json(json& j, portals const& x)
    {
        j = json::object();
        for (auto const& [key, value] : x.get_data())
        {
            j[key] = value;
        }
    }

    inline void from_json(json const& j, mapJson& x)
    {
        x.set_name(j.at("name").get<std::string>());
        x.set_width(j.at("width").get<int64_t>());
        x.set_height(j.at("height").get<int64_t>());
        x.set_layout(j.at("layout").get<std::vector<std::string>>());
        x.set_scatter_points(j.at("scatterPoints").get<std::map<std::string, std::vector<int64_t>>>());
        x.set_portals(j.at("portals").get<portals>());
        x.set_ghosts(j.at("ghosts").get<std::vector<ghost>>());
    }

    inline void to_json(json& j, mapJson const& x)
    {
        j = json::object();
        j["name"] = x.get_name();
        j["width"] = x.get_width();
        j["height"] = x.get_height();
        j["layout"] = x.get_layout();
        j["scatterPoints"] = x.get_scatter_points();
        j["portals"] = x.get_portals();
        j["ghosts"] = x.get_ghosts();
    }
} // namespace quicktype
