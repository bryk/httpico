/**
 =========================================================================================
 File name	: /httpico/include/HttpServer.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpServer.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "HttpRequestProcessor/SocketReaderHttpRequestProcessor.hpp"
#include "HttpRequest.hpp"
#include "HttpServerConfiguration.hpp"

using boost::asio::ip::tcp;

namespace Httpico {

HttpServer::HttpServer(HttpServerConfiguration &conf) :
		configuration(conf), io_service(), acceptor(io_service, tcp::endpoint(tcp::v4(), configuration.getServerPort())) {
}

HttpServer::~HttpServer() {
	// TODO Auto-generated destructor stub
}

void HttpServer::handleAccept(socketPtr sock) {
	std::cerr << "accepted socket\n";
	acceptNewSocket();
	HttpRequestPtr req(new HttpRequest(sock));
	HttpRequestProcessorPtr ptr(new SocketReaderHttpRequestProcessor(req, *this));
	ptr->process();
	//ptr->process();
	//char buf[10000];

	/*boost::system::error_code error;
	 sock->read_some(boost::asio::buffer(buf), error);
	 if (error == boost::asio::error::eof)
	 return; // Connection closed cleanly by peer.
	 else if (error)
	 throw boost::system::system_error(error); // Some other error.

	 std::cerr << buf << std::endl;*/

	//sock->async_read_some(boost::asio::buffer(buf), boost::bind(&HttpServer::handleRead, this, sock));
	//io_service.reset();
}
/*
 void HttpServer::handleRead(boost::shared_ptr<tcp::socket> sock) {
 std::cerr << buf.elems << std::endl;
 std::ostringstream ss;
 ss << num++;
 std::string message = ss.str();

 sock->shutdown(tcp::socket::shutdown_receive);

 boost::asio::async_write(*sock, boost::asio::buffer(message),
 boost::bind(&HttpServer::handleWrite, this, boost::asio::placeholders::error,
 boost::asio::placeholders::bytes_transferred, sock));
 }

 void HttpServer::handleWrite(const boost::system::error_code& , size_t bytes_written,
 boost::shared_ptr<tcp::socket> sock) {
 std::cerr << "handleWrite, zapisano: " << bytes_written << std::endl;
 sock->shutdown(tcp::socket::shutdown_send);
 sock.reset();
 }*/

void HttpServer::acceptNewSocket() {
	socketPtr sock(new tcp::socket(io_service));
	acceptor.async_accept(*sock, boost::bind(&HttpServer::handleAccept, this, sock));
}

void HttpServer::start() {
	std::cerr << "starting server\n";
	try {
		std::cerr << "aaaaa" << std::endl;
		acceptNewSocket();
		std::cerr << "sssss" << std::endl;
		//while (true) {
		//std::cerr << "Loop" << std::endl;

		io_service.run();
		//	}
	} catch (std::exception& e) {
		std::cerr << "Overall exception" << e.what() << std::endl;
	}
}

} //namespace
