/**
 =========================================================================================
 File name	: /httpico/include/HttpServer.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpServer.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include "HttpRequestProcessor/SocketReaderHttpRequestProcessor.hpp"
#include "HttpRequest.hpp"

using boost::asio::ip::tcp;

namespace Httpico {

HttpServer::HttpServer(HttpServerConfiguration &conf) :
		configuration(conf) {
}

HttpServer::~HttpServer() {
	// TODO Auto-generated destructor stub
}

void HttpServer::start() {
	std::cerr << "starting server\n";
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), configuration.getServerPort()));
		while (true) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);

			std::string message = "ABC222";
			HttpRequestPtr req(new HttpRequest());
			HttpRequestProcessorPtr ptr(new SocketReaderHttpRequestProcessor(req));
			ptr->process();
			char buf[10000];

			boost::system::error_code error;
			socket.read_some(boost::asio::buffer(buf), error);
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			std::cerr << buf << std::endl;

			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

} //namespace
