#include <iostream>

#include "Network/NetworkManager.h"
#include "VK/API/VKClient.hpp"

int main()
{
    VK::Client client;
    std::cout << (client.Authorization("aefbd995219ba3151282696e2b78a842b95822d03a9ef2db98a53bc947373826ff48960e3d0f184d643f2") ? client.GetClientName() : "FUCK");
}