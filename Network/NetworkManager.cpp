#include <iostream>

#include "NetworkManager.h"

Network::NetworkManager::NetworkManager(const size_t numThreads) noexcept
#if defined(SIGQUIT)
        : signals(ioc, SIGINT, SIGTERM, SIGQUIT)
#else
        : signals(ioc, SIGINT, SIGTERM)
#endif
{
    signals.async_wait([this](boost::system::error_code const&, int) { ioc.stop(); });

    ctx.set_verify_mode(ssl::verify_peer);

    threads.reserve(numThreads);

    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.emplace_back([this] { run(); });
    }
}

void Network::NetworkManager::request(boost::string_view host, http::verb method, boost::string_view target,
        const RequestParams &params, const Handler &handler) noexcept
{
    // TODO: URL

    HttpSession session(ioc, ctx, handler);

    session.asyncRequest(method, host, target);
}

void Network::NetworkManager::run() noexcept
{
    while (true)
    {
        try
        {
            ioc.run();

            break;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
