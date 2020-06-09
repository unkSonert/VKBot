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

    ctx.set_default_verify_paths();

    threads.reserve(numThreads);

    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.emplace_back([this] { run(); });
    }
}

void Network::NetworkManager::request(const http::verb method, const network::uri &uri, Handler &&handler) noexcept
{
    HttpSession session(ioc, ctx, std::move(handler));

    auto authorityView = uri.authority();
    auto pathView = uri.path();
    auto queryView = uri.query();

    if (pathView.empty())
    {
        pathView = "/";
    }

    std::string path;
    path.reserve(pathView.size() + queryView.size() + 1);

    path.append(pathView.data(), pathView.size());
    path += '?';
    path.append(queryView.data(), queryView.size());

    session.asyncRequest(method, { authorityView.data(), authorityView.size() }, path);
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

Network::NetworkManager::~NetworkManager() noexcept
{
    ioc.stop();

    for (auto &thread : threads)
    {
        thread.join();
    }
}
