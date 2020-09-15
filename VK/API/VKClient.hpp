#pragma once

#include <string>
#include <future>
#include <unordered_map>
#include <utility>
#include "nlohmann/json.hpp"
#include "Network/NetworkManager.h"
#include "User.hpp"
using json = ::nlohmann::json;

namespace VK
{

    class Client
    {
    private:
        const std::string apiUrl = "api.vk.com";
        const std::string appId = "3697615";//"3140623";// android=2274003
        const std::string appSecret = "AlVXZFMUqyrnABp8ncuU";//"VeWdmVclDCtn6ihuP1nt";// android=hHbZxrka2uZ6jB1inYsH
        const std::string scope = "offline,groups,messages,friends,audio,wall";
        const std::string authUrl = "oauth.vk.com";

        Network::NetworkManager manager = Network::NetworkManager(4);

        std::string version;
        std::string language;
        std::string accessToken;

        Api::User userObject;

        bool CheckAccess()
        {
            json result = MethodCall("users.get", {});
            if (result.find("error") != result.end())
                return false;

            try
            {
                userObject = Api::User(result.at("response").get<json>().begin()->get<json>());
            }
            catch (...)
            {
                return false;
            }

            return true;
        }

    public:

        bool Authorization(const std::string &login, const std::string &password)
        {
            std::promise<std::string> promise;
            auto future = promise.get_future();

            network::uri uri;
            auto builder = network::uri_builder(uri);

            builder.authority(authUrl).path("token")
                    .append_query_key_value_pair("client_id", appId)
                    .append_query_key_value_pair("grant_type", "password")
                    .append_query_key_value_pair("client_secret", appSecret)
                    .append_query_key_value_pair("scope", scope)
                    .append_query_key_value_pair("username", login)
                    .append_query_key_value_pair("password", password);

            manager.request(http::verb::get, builder.uri(),
                            [&](Network::ErrorCode &&error, http::response<http::string_body> &&response,
                                std::string &&result)
                            {
                                if (error.ec)
                                {
                                    promise.set_value(R"("error": "network")");
                                    return;
                                }

                                promise.set_value(result);
                            });

            json result = json::parse(future.get());

            if (result.find("error") != result.end())
                return false;

            try
            {
                if (result["expires_in"].get<int>() != 0)
                    return false;

                accessToken = result["access_token"].get<std::string>();
            }
            catch (...)
            {
                return false;
            }

            return CheckAccess();
        }

        bool Authorization(std::string token)
        {
            accessToken = std::move(token);
            return CheckAccess();
        }

        json MethodCall(const std::string &method, const std::unordered_map<std::string, std::string> &params)
        {
            std::promise<json> promise;
            auto future = promise.get_future();

            network::uri uri;
            auto builder = network::uri_builder(uri);

            builder.authority(apiUrl).path("method/" + method);

            for (auto& [key, param] : params)
                builder.append_query_key_value_pair(key, param);

            builder.append_query_key_value_pair("access_token", accessToken);
            builder.append_query_key_value_pair("v", version);
            builder.append_query_key_value_pair("lang", language);

            manager.request(http::verb::get, builder.uri(),
                            [&](Network::ErrorCode &&error, http::response<http::string_body> &&response,
                                std::string &&result)
                            {
                                if (error.ec)
                                {
                                    promise.set_value(json::parse(R"("error": "network")"));
                                    return;
                                }

                                promise.set_value(json::parse(result));
                            });

            return future.get();
        }

        explicit Client(std::string version = "5.65", std::string language = "en") : version(std::move(version)),
                                                                                     language(std::move(language)) {}

        std::string getAccessToken() { return accessToken; }

        std::string GetClientName() { return userObject.GetFullName(); }
    };
}