/**
 =========================================================================================
 File name	: /httpico/include/HttpServer.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef POINTER_DEFINITIONS_HPP_
#define POINTER_DEFINITIONS_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace Httpico {
using boost::asio::ip::tcp;
class HttpRequest;
class HttpRequestProcessor;
class HttpServer;
class HttpServerConfiguration;
typedef boost::shared_ptr<tcp::socket> socketPtr;
typedef boost::shared_ptr<HttpRequest> HttpRequestPtr;
typedef boost::shared_ptr<HttpRequestProcessor> HttpRequestProcessorPtr;
}
#endif 
