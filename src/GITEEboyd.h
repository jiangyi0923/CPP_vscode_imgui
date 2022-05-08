//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     quicktype2::GiteEboyd data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

namespace quicktype2 {
    using nlohmann::json;

    inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }

    class File {
    public:
        File() = default;
        virtual ~File() = default;

    private:
        int id;
        std::string name;
        std::string md5;
        int size;

    public:
        const int& get_id() const { return id; }
        int& get_mutable_id() { return id; }
        void set_id(const int& value) { this->id = value; }

        const std::string& get_name() const { return name; }
        std::string& get_mutable_name() { return name; }
        void set_name(const std::string& value) { this->name = value; }

        const std::string& get_md5() const { return md5; }
        std::string& get_mutable_md5() { return md5; }
        void set_md5(const std::string& value) { this->md5 = value; }

        const int& get_size() const { return size; }
        int& get_mutable_size() { return size; }
        void set_size(const int& value) { this->size = value; }
    };

    class GiteEboyd {
    public:
        GiteEboyd() = default;
        virtual ~GiteEboyd() = default;

    private:
        int yer;
        int mos;
        int days;
        std::string fileday;
        std::string dxmd5;
        std::vector<quicktype2::File> files;

    public:
        const int& get_yer() const { return yer; }
        int& get_mutable_yer() { return yer; }
        void set_yer(const int& value) { this->yer = value; }

        const int& get_mos() const { return mos; }
        int& get_mutable_mos() { return mos; }
        void set_mos(const int& value) { this->mos = value; }

        const int& get_days() const { return days; }
        int& get_mutable_days() { return days; }
        void set_days(const int& value) { this->days = value; }

        const std::string& get_fileday() const { return fileday; }
        std::string& get_mutable_fileday() { return fileday; }
        void set_fileday(const std::string& value) { this->fileday = value; }

        const std::string& get_dxmd5() const { return dxmd5; }
        std::string& get_mutable_dxmd5() { return dxmd5; }
        void set_dxmd5(const std::string& value) { this->dxmd5 = value; }

        const std::vector<quicktype2::File>& get_files() const { return files; }
        std::vector<quicktype2::File>& get_mutable_files() { return files; }
        void set_files(const std::vector<quicktype2::File>& value) { this->files = value; }
    };
}

namespace nlohmann {
    namespace detail {
        void from_json(const json& j, quicktype2::File& x);
        void to_json(json& j, const quicktype2::File& x);

        void from_json(const json& j, quicktype2::GiteEboyd& x);
        void to_json(json& j, const quicktype2::GiteEboyd& x);

        inline void from_json(const json& j, quicktype2::File& x) {
            x.set_id(j.at("id").get<int>());
            x.set_name(j.at("name").get<std::string>());
            x.set_md5(j.at("md5").get<std::string>());
            x.set_size(j.at("size").get<int>());
        }

        inline void to_json(json& j, const quicktype2::File& x) {
            j = json::object();
            j["id"] = x.get_id();
            j["name"] = x.get_name();
            j["md5"] = x.get_md5();
            j["size"] = x.get_size();
        }

        inline void from_json(const json& j, quicktype2::GiteEboyd& x) {
            x.set_yer(j.at("yer").get<int>());
            x.set_mos(j.at("mos").get<int>());
            x.set_days(j.at("days").get<int>());
            x.set_fileday(j.at("fileday").get<std::string>());
            x.set_dxmd5(j.at("dxmd5").get<std::string>());
            x.set_files(j.at("files").get<std::vector<quicktype2::File>>());
        }

        inline void to_json(json& j, const quicktype2::GiteEboyd& x) {
            j = json::object();
            j["yer"] = x.get_yer();
            j["mos"] = x.get_mos();
            j["days"] = x.get_days();
            j["fileday"] = x.get_fileday();
            j["dxmd5"] = x.get_dxmd5();
            j["files"] = x.get_files();
        }
    }
}
