#pragma once

#include <boost/noncopyable.hpp>

#include "network/uri.hpp"

#include "boostNetwork.h"
#include "HttpSession.h"

namespace Network
{
    class NetworkManager : private boost::noncopyable
    {
    public:
        using RequestParams = std::unordered_map<std::string, std::string>;

        explicit NetworkManager(size_t numThreads) noexcept;

        void request(const http::verb method, const network::uri &uri, Handler &&handler) noexcept;

        ~NetworkManager() noexcept;

    private:
        net::io_context ioc;

        ssl::context ctx{ ssl::context::tlsv12_client };

        net::signal_set signals;

        std::vector<std::thread> threads;

        void run() noexcept;
    };
}
