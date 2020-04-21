#pragma once
#include <string>
#include <deque>
#include <functional>

using RequestParams = std::deque<std::pair<std::string, std::string>>;

class NetworkManager
{
	static void Request(std::string request, RequestParams params, std::function<void(std::string)> answerResolver = nullptr)
	{

	}
};