//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
// Bad code Copyright 2017 Tim Simpson
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP SSL client, coroutine
//
//------------------------------------------------------------------------------

#include "certs.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

int main(int argc, char** argv)
{
    // Check command line arguments.
    if(argc != 4 && argc != 5)
    {
        std::cerr <<
            "Usage: http-client-coro-ssl <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]\n" <<
            "Example:\n" <<
            "    http-client-coro-ssl www.example.com 443 /\n" <<
            "    http-client-coro-ssl www.example.com 443 / 1.0\n";
        return EXIT_FAILURE;
    }
    auto const host = argv[1];
    auto const port = argv[2];
    auto const target = argv[3];
    int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

    boost::asio::io_service ios;

    ssl::context ctx{ssl::context::sslv23_client};

    load_root_certificates(ctx);
    
    boost::asio::spawn(ios, [&](boost::asio::yield_context yield)
    {
        tcp::resolver resolver{ios};
        ssl::stream<tcp::socket> stream{ios, ctx};

		{
			auto const begin = resolver.async_resolve(
				tcp::resolver::query{ host, port }, yield);
			tcp::resolver::iterator end;
			boost::asio::async_connect(
				stream.next_layer(), begin, end, yield);
		}

        stream.async_handshake(ssl::stream_base::client, yield);

        http::request<http::string_body> req{http::verb::get, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::async_write(stream, req, yield);

        boost::beast::flat_buffer b;

        http::response<http::dynamic_body> res;

        http::async_read(stream, b, res, yield);

        std::cout << res << std::endl;
		
		boost::system::error_code ec;
		// This is currently failing every single time due to this bug: 
		//	https://svn.boost.org/trac10/ticket/12710
        stream.async_shutdown(yield[ec]);
        if(ec != boost::asio::error::eof)
        {            
			boost::asio::detail::throw_error(ec);
		}
    });

    ios.run();
    

    return EXIT_SUCCESS;
}
