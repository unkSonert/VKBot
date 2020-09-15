#include <iostream>

#include "Network/NetworkManager.h"
#include "VK/API/VKClient.hpp"
#include "VK/API/VKLongPoll.hpp"

int main()
{
    VK::Client client;
    client.Authorization("aefbd995219ba3151282696e2b78a842b95822d03a9ef2db98a53bc947373826ff48960e3d0f184d643f2");

    auto LP = VK::LongPoll(&client);
    LP.AddLPHandler([](const json& update){std::cout << update << std::endl; return true;});

    while (1)
        sleep(200);
}