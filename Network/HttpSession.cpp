#include "HttpSession.h"

#include <utility>
#include "HttpSessionImpl.h"

Network::HttpSession::HttpSession(net::io_context &ioc, ssl::context &ctx, Handler &&handler) noexcept
        : impl(new HttpSessionImpl(ioc, ctx, std::move(handler)))
{}

void Network::HttpSession::asyncRequest(const http::verb method, const boost::string_view host,
        const boost::string_view target) noexcept
{ impl->asyncRequest(method, host, target); }

BOOST_FORCEINLINE Network::HttpSessionImpl::HttpSessionImpl(net::io_context &ioc, ssl::context &ctx,
        Handler &&handler) noexcept
        : resolver(net::make_strand(ioc)), stream(net::make_strand(ioc), ctx), handler(std::move(handler))
{}

BOOST_FORCEINLINE void Network::HttpSessionImpl::asyncRequest(const http::verb method, const boost::string_view host,
        const boost::string_view target) noexcept
{
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.data()))
    {
        call_handler({ static_cast<int>(::ERR_get_error()), net::error::get_ssl_category() }, "set tlsext host name");

        return;
    }

    request.method(method);
    request.target(target);
    request.set(http::field::host, host);

    resolver.async_resolve({host.data(), host.size()}, "443",
            [this](const boost::system::error_code ec, const tcp::resolver::results_type& results)
    {
        if (ec)
        {
            call_handler(ec, "resolve");

            return;
        }

        beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

        beast::get_lowest_layer(stream).async_connect(results,
                [this](beast::error_code ec, const tcp::resolver::results_type::endpoint_type&)
        {
            if (ec)
            {
                call_handler(ec, "connect");

                return;
            }

            stream.async_handshake(ssl::stream_base::client, [this](beast::error_code ec)
            {
                if (ec)
                {
                    call_handler(ec, "handshake");

                    return;
                }

                beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

                http::async_write(stream, request, [this](beast::error_code ec, std::size_t bytes_transferred)
                {
                    if (ec)
                    {
                        call_handler(ec, "write");

                        return;
                    }

                    http::async_read(stream, buffer, response,
                            [this](beast::error_code ec, std::size_t bytes_transferred)
                    {
                        call_handler(ec, ec ? "read" : "");
                    });
                });
            });
        });
    });
}

void Network::HttpSessionImpl::closeSocket()
{
    stream.async_shutdown([this](beast::error_code ec)
    {
        if (ec == net::error::eof)
        {
            ec = {};
        }

        if (ec)
        {
            handler({ ec, ("shutdown") }, std::move(response), std::string());
        }

        delete this;
    });
}
