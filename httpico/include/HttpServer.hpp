/**
 =========================================================================================
 File name	: /httpico/include/HttpServer.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPSERVER_HPP_
#define HTTPSERVER_HPP_

#include "PionterDefinitions.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace Httpico {

using boost::asio::ip::tcp;
typedef boost::shared_ptr<tcp::socket> socketPtr;

/*
 * Httpico::HttpServer
 */
class HttpServer {
public:
	HttpServer(HttpServerConfiguration &conf);
	virtual ~HttpServer();
	void start();
	void runProcessorsLoop();

private:
	HttpServerConfiguration &configuration;
	void handleAccept(socketPtr sock);
	//void handleWrite(const boost::system::error_code& /*error*/, size_t bytes_written, boost::shared_ptr<tcp::socket> sock);
	//void handleRead(boost::shared_ptr<tcp::socket> sock);
	//boost::array<char, 8192> buf;
	boost::asio::io_service io_service;
	tcp::acceptor acceptor;
	void acceptNewSocket();
};

}
#endif 
