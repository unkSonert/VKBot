#pragma once

#include <string>
#include "nlohmann/json.hpp"

using json = ::nlohmann::json;

namespace VK::Api
{
    class User
    {
    private:
        int uid = -1;

        std::string firstName;
        std::string lastName;

    public:
        User() = default;

        explicit User(json userData)
        {
            uid = userData.at("id").get<int>();
            firstName = userData.at("first_name").get<std::string>();
            lastName = userData.at("last_name").get<std::string>();
        }

        int GetUid() const { return uid; }

        std::string GetFullName() { return firstName + " " + lastName; }

        bool operator==(const User &cmp)
        {
            return uid == cmp.uid;
        }

        bool operator!=(const User &cmp)
        {
            return !(*this == cmp);
        }
    };
}