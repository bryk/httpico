/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor/SocketReaderHttpRequestProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequestProcessor/SocketReaderHttpRequestProcessor.hpp"
#include "HttpRequest.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

namespace Httpico {

SocketReaderHttpRequestProcessor::SocketReaderHttpRequestProcessor(HttpRequestPtr httpRequest, HttpServer &srv) :
		HttpRequestProcessor(httpRequest, srv) {
}

SocketReaderHttpRequestProcessor::~SocketReaderHttpRequestProcessor() {
}

int num = 0;

void SocketReaderHttpRequestProcessor::readHandler(const boost::system::error_code& error, std::size_t bytesTransferred) {
	httpRequest->getSocket()->shutdown(tcp::socket::shutdown_receive);
	std::string mm(buf.elems, bytesTransferred);
	std::cerr << "wczytano:\n" << mm << std::endl;

	boost::asio::async_write(*(httpRequest->getSocket()), boost::asio::buffer(mm),
			boost::bind(&SocketReaderHttpRequestProcessor::writeHandler, shared_from_this(),
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void SocketReaderHttpRequestProcessor::writeHandler(const boost::system::error_code& error, std::size_t bytesTransferred) {
	std::cerr << "zapisano\n";
	httpRequest->getSocket()->shutdown(tcp::socket::shutdown_send);
}

void SocketReaderHttpRequestProcessor::process() {
	std::cerr << "process\n";
	socketPtr sock = httpRequest->getSocket();
	sock->async_read_some(boost::asio::buffer(buf),
			boost::bind(&SocketReaderHttpRequestProcessor::readHandler, shared_from_this(), boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

}

}
