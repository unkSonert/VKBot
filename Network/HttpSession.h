#pragma once

#include "boostNetwork.h"

namespace Network
{
    struct ErrorCode
    {
        boost::system::error_code ec;
        boost::string_view stepName;
    };

    using Handler = std::function<void(ErrorCode &&, http::response<http::string_body> &&, std::string &&)>;

    class HttpSessionImpl;

    class HttpSession
    {
    public:
        HttpSession(net::io_context &ioc, ssl::context &ctx, Handler handler) noexcept;

        void asyncRequest(const http::verb method, const boost::string_view host,
                const boost::string_view target) noexcept;

    private:
        HttpSessionImpl *impl;
    };
}
