#include <iostream>

#include "Network/NetworkManager.h"

int main()
{
    Network::NetworkManager manager(4);

    network::uri uri;
    auto builder = network::uri_builder(uri);

    builder.path("method/users.get").authority("api.vk.com").append_query_key_value_pair("user_ids", "210700286");

    bool flag = true;

    manager.request(http::verb::get, builder.uri(), [&flag] (auto &&one, auto &&two, auto &&three)
    {
        std::cout << three;

        flag = false;
    });

    while (flag);
}