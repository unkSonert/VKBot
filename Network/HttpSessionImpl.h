#pragma once

#include "boostNetwork.h"
#include "HttpSession.h"

namespace Network
{
    class HttpSessionImpl
    {
    private:
        friend class HttpSession;

        tcp::resolver resolver;
        beast::ssl_stream<beast::tcp_stream> stream;
        beast::flat_buffer buffer;
        http::request<http::empty_body> request;
        http::response<http::string_body> response;

        Handler handler;

        HttpSessionImpl(net::io_context &ioc, ssl::context &ctx, Handler &&handler) noexcept;

        void asyncRequest(http::verb method, boost::string_view host, boost::string_view target) noexcept;

        inline void call_handler(const boost::system::error_code ec, const boost::string_view stepName)
        {
            handler({ ec, stepName }, std::move(response), std::move(response.body()));

            closeSocket();
        }

        void closeSocket();
    };
}
