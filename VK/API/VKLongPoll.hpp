#pragma once
#include <string>
#include "../../Network/NetworkManager.h"
#include "VKClient.hpp"
#include <vector>

namespace VK
{
    class LongPoll
    {
    private:
        std::string server;
        std::string key;
        unsigned long ts;

        int wait;
        int mode;

        Client* VkClient;
        Network::NetworkManager manager = Network::NetworkManager(1);

        std::vector<std::function<bool(json)>> lpHandlers;

        void UpdateLPInfo()
        {
            if (!VkClient) throw "Meow";

            auto result = VkClient->MethodCall("messages.getLongPollServer", {{"lp_version", "3"}});

            if (result.find("response") == result.end()) throw "Meow";

            result = result.at("response").get<json>();

            server = result.at("server").get<std::string>();
            key = result.at("key").get<std::string>();
            ts = result.at("ts").get<unsigned long>();
        }

        void Polling()
        {
            network::uri uri("https://" + server);
            auto builder = network::uri_builder(uri);

            builder.append_query_key_value_pair("act", "a_check")
                    .append_query_key_value_pair("key", key)
                    .append_query_key_value_pair("ts", std::to_string(ts).c_str())
                    .append_query_key_value_pair("wait", std::to_string(wait).c_str())
                    .append_query_key_value_pair("mode", std::to_string(mode).c_str())
                    .append_query_key_value_pair("version", "3");

            manager.request(http::verb::get, builder.uri(),
                            [&](Network::ErrorCode &&error, http::response<http::string_body> &&response,
                                std::string &&result)
                            {
                                if (error.ec)
                                {
                                    //TODO: check network memes
                                    return;
                                }

                                auto lpMessage = json::parse(result);

                                if (lpMessage.find("failed") != lpMessage.end())
                                {
                                    auto failCode = lpMessage.at("failed").get<int>();

                                    if (failCode == 2 || failCode == 3)
                                        UpdateLPInfo();

                                    if (failCode == 1)
                                        ts = lpMessage.at("ts").get<unsigned long>();

                                    Polling();
                                    return;
                                }

                                if (lpMessage.find("ts") == lpMessage.end()) throw "Meow";

                                ts = lpMessage.at("ts").get<unsigned long>();

                                if (lpMessage.find("updates") == lpMessage.end()) throw "Meow";

                                auto updates = lpMessage.at("updates");

                                for (const auto& update : updates)
                                {
                                    for (const auto& handler : lpHandlers)
                                    {
                                        if (handler(update))
                                            break;
                                    }
                                }

                                Polling();
                            });
        }

    public:
        explicit LongPoll(Client* vkClient, int waitTime = 25, int lpMode = 2 | 8)
        {
            VkClient = vkClient;

            wait = waitTime;
            mode = lpMode;

            UpdateLPInfo();

            Polling();
        }

        void AddLPHandler(std::function<bool(json)> handler)
        {
            lpHandlers.push_back(handler);
        }
    };
}