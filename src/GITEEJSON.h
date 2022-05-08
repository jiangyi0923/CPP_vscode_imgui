//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     quicktype::Giteejson data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json& j, const std::shared_ptr<T>& opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json& j) {
            if (j.is_null()) return std::unique_ptr<T>(); else return std::unique_ptr<T>(new T(j.get<T>()));
        }
    };
}
#endif

namespace quicktype {
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

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json& j, std::string property) {
        return get_optional<T>(j, property.data());
    }

    class Asset {
    public:
        Asset() = default;
        virtual ~Asset() = default;

    private:
        std::string browser_download_url;
        std::shared_ptr<std::string> name;

    public:
        const std::string& get_browser_download_url() const { return browser_download_url; }
        std::string& get_mutable_browser_download_url() { return browser_download_url; }
        void set_browser_download_url(const std::string& value) { this->browser_download_url = value; }

        std::shared_ptr<std::string> get_name() const { return name; }
        void set_name(std::shared_ptr<std::string> value) { this->name = value; }
    };

    class Author {
    public:
        Author() = default;
        virtual ~Author() = default;

    private:
        int id;
        std::string login;
        std::string name;
        std::string avatar_url;
        std::string url;
        std::string html_url;
        std::string remark;
        std::string followers_url;
        std::string following_url;
        std::string gists_url;
        std::string starred_url;
        std::string subscriptions_url;
        std::string organizations_url;
        std::string repos_url;
        std::string events_url;
        std::string received_events_url;
        std::string type;

    public:
        const int& get_id() const { return id; }
        int& get_mutable_id() { return id; }
        void set_id(const int& value) { this->id = value; }

        const std::string& get_login() const { return login; }
        std::string& get_mutable_login() { return login; }
        void set_login(const std::string& value) { this->login = value; }

        const std::string& get_name() const { return name; }
        std::string& get_mutable_name() { return name; }
        void set_name(const std::string& value) { this->name = value; }

        const std::string& get_avatar_url() const { return avatar_url; }
        std::string& get_mutable_avatar_url() { return avatar_url; }
        void set_avatar_url(const std::string& value) { this->avatar_url = value; }

        const std::string& get_url() const { return url; }
        std::string& get_mutable_url() { return url; }
        void set_url(const std::string& value) { this->url = value; }

        const std::string& get_html_url() const { return html_url; }
        std::string& get_mutable_html_url() { return html_url; }
        void set_html_url(const std::string& value) { this->html_url = value; }

        const std::string& get_remark() const { return remark; }
        std::string& get_mutable_remark() { return remark; }
        void set_remark(const std::string& value) { this->remark = value; }

        const std::string& get_followers_url() const { return followers_url; }
        std::string& get_mutable_followers_url() { return followers_url; }
        void set_followers_url(const std::string& value) { this->followers_url = value; }

        const std::string& get_following_url() const { return following_url; }
        std::string& get_mutable_following_url() { return following_url; }
        void set_following_url(const std::string& value) { this->following_url = value; }

        const std::string& get_gists_url() const { return gists_url; }
        std::string& get_mutable_gists_url() { return gists_url; }
        void set_gists_url(const std::string& value) { this->gists_url = value; }

        const std::string& get_starred_url() const { return starred_url; }
        std::string& get_mutable_starred_url() { return starred_url; }
        void set_starred_url(const std::string& value) { this->starred_url = value; }

        const std::string& get_subscriptions_url() const { return subscriptions_url; }
        std::string& get_mutable_subscriptions_url() { return subscriptions_url; }
        void set_subscriptions_url(const std::string& value) { this->subscriptions_url = value; }

        const std::string& get_organizations_url() const { return organizations_url; }
        std::string& get_mutable_organizations_url() { return organizations_url; }
        void set_organizations_url(const std::string& value) { this->organizations_url = value; }

        const std::string& get_repos_url() const { return repos_url; }
        std::string& get_mutable_repos_url() { return repos_url; }
        void set_repos_url(const std::string& value) { this->repos_url = value; }

        const std::string& get_events_url() const { return events_url; }
        std::string& get_mutable_events_url() { return events_url; }
        void set_events_url(const std::string& value) { this->events_url = value; }

        const std::string& get_received_events_url() const { return received_events_url; }
        std::string& get_mutable_received_events_url() { return received_events_url; }
        void set_received_events_url(const std::string& value) { this->received_events_url = value; }

        const std::string& get_type() const { return type; }
        std::string& get_mutable_type() { return type; }
        void set_type(const std::string& value) { this->type = value; }
    };

    class Giteejson {
    public:
        Giteejson() = default;
        virtual ~Giteejson() = default;

    private:
        int id;
        std::string tag_name;
        std::string target_commitish;
        bool prerelease;
        std::string name;
        std::string body;
        quicktype::Author author;
        std::string created_at;
        std::vector<quicktype::Asset> assets;

    public:
        const int& get_id() const { return id; }
        int& get_mutable_id() { return id; }
        void set_id(const int& value) { this->id = value; }

        const std::string& get_tag_name() const { return tag_name; }
        std::string& get_mutable_tag_name() { return tag_name; }
        void set_tag_name(const std::string& value) { this->tag_name = value; }

        const std::string& get_target_commitish() const { return target_commitish; }
        std::string& get_mutable_target_commitish() { return target_commitish; }
        void set_target_commitish(const std::string& value) { this->target_commitish = value; }

        const bool& get_prerelease() const { return prerelease; }
        bool& get_mutable_prerelease() { return prerelease; }
        void set_prerelease(const bool& value) { this->prerelease = value; }

        const std::string& get_name() const { return name; }
        std::string& get_mutable_name() { return name; }
        void set_name(const std::string& value) { this->name = value; }

        const std::string& get_body() const { return body; }
        std::string& get_mutable_body() { return body; }
        void set_body(const std::string& value) { this->body = value; }

        const quicktype::Author& get_author() const { return author; }
        quicktype::Author& get_mutable_author() { return author; }
        void set_author(const quicktype::Author& value) { this->author = value; }

        const std::string& get_created_at() const { return created_at; }
        std::string& get_mutable_created_at() { return created_at; }
        void set_created_at(const std::string& value) { this->created_at = value; }

        const std::vector<quicktype::Asset>& get_assets() const { return assets; }
        std::vector<quicktype::Asset>& get_mutable_assets() { return assets; }
        void set_assets(const std::vector<quicktype::Asset>& value) { this->assets = value; }
    };
}

namespace nlohmann {
    namespace detail {
        void from_json(const json& j, quicktype::Asset& x);
        void to_json(json& j, const quicktype::Asset& x);

        void from_json(const json& j, quicktype::Author& x);
        void to_json(json& j, const quicktype::Author& x);

        void from_json(const json& j, quicktype::Giteejson& x);
        void to_json(json& j, const quicktype::Giteejson& x);

        inline void from_json(const json& j, quicktype::Asset& x) {
            x.set_browser_download_url(j.at("browser_download_url").get<std::string>());
            x.set_name(quicktype::get_optional<std::string>(j, "name"));
        }

        inline void to_json(json& j, const quicktype::Asset& x) {
            j = json::object();
            j["browser_download_url"] = x.get_browser_download_url();
            j["name"] = x.get_name();
        }

        inline void from_json(const json& j, quicktype::Author& x) {
            x.set_id(j.at("id").get<int>());
            x.set_login(j.at("login").get<std::string>());
            x.set_name(j.at("name").get<std::string>());
            x.set_avatar_url(j.at("avatar_url").get<std::string>());
            x.set_url(j.at("url").get<std::string>());
            x.set_html_url(j.at("html_url").get<std::string>());
            x.set_remark(j.at("remark").get<std::string>());
            x.set_followers_url(j.at("followers_url").get<std::string>());
            x.set_following_url(j.at("following_url").get<std::string>());
            x.set_gists_url(j.at("gists_url").get<std::string>());
            x.set_starred_url(j.at("starred_url").get<std::string>());
            x.set_subscriptions_url(j.at("subscriptions_url").get<std::string>());
            x.set_organizations_url(j.at("organizations_url").get<std::string>());
            x.set_repos_url(j.at("repos_url").get<std::string>());
            x.set_events_url(j.at("events_url").get<std::string>());
            x.set_received_events_url(j.at("received_events_url").get<std::string>());
            x.set_type(j.at("type").get<std::string>());
        }

        inline void to_json(json& j, const quicktype::Author& x) {
            j = json::object();
            j["id"] = x.get_id();
            j["login"] = x.get_login();
            j["name"] = x.get_name();
            j["avatar_url"] = x.get_avatar_url();
            j["url"] = x.get_url();
            j["html_url"] = x.get_html_url();
            j["remark"] = x.get_remark();
            j["followers_url"] = x.get_followers_url();
            j["following_url"] = x.get_following_url();
            j["gists_url"] = x.get_gists_url();
            j["starred_url"] = x.get_starred_url();
            j["subscriptions_url"] = x.get_subscriptions_url();
            j["organizations_url"] = x.get_organizations_url();
            j["repos_url"] = x.get_repos_url();
            j["events_url"] = x.get_events_url();
            j["received_events_url"] = x.get_received_events_url();
            j["type"] = x.get_type();
        }

        inline void from_json(const json& j, quicktype::Giteejson& x) {
            x.set_id(j.at("id").get<int>());
            x.set_tag_name(j.at("tag_name").get<std::string>());
            x.set_target_commitish(j.at("target_commitish").get<std::string>());
            x.set_prerelease(j.at("prerelease").get<bool>());
            x.set_name(j.at("name").get<std::string>());
            x.set_body(j.at("body").get<std::string>());
            x.set_author(j.at("author").get<quicktype::Author>());
            x.set_created_at(j.at("created_at").get<std::string>());
            x.set_assets(j.at("assets").get<std::vector<quicktype::Asset>>());
        }

        inline void to_json(json& j, const quicktype::Giteejson& x) {
            j = json::object();
            j["id"] = x.get_id();
            j["tag_name"] = x.get_tag_name();
            j["target_commitish"] = x.get_target_commitish();
            j["prerelease"] = x.get_prerelease();
            j["name"] = x.get_name();
            j["body"] = x.get_body();
            j["author"] = x.get_author();
            j["created_at"] = x.get_created_at();
            j["assets"] = x.get_assets();
        }
    }
}
