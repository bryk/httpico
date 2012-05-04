/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor/SocketReaderHttpRequestProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef SOCKETREADERHTTPREQUESTPROCESSOR_HPP_
#define SOCKETREADERHTTPREQUESTPROCESSOR_HPP_

#include "HttpRequestProcessor.hpp"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace Httpico {

/*
 * Httpico::SocketReaderHttpRequestProcessor
 */
class SocketReaderHttpRequestProcessor: public Httpico::HttpRequestProcessor, public boost::enable_shared_from_this<
		SocketReaderHttpRequestProcessor> {
public:
	SocketReaderHttpRequestProcessor(HttpRequestPtr httpRequest, HttpServer &srv);
	virtual ~SocketReaderHttpRequestProcessor();
	virtual void process();
	void readHandler(const boost::system::error_code& error, std::size_t bytesTransferred);
	void writeHandler(const boost::system::error_code& error, std::size_t bytesTransferred);
private:
	boost::array<char, 8192> buf;
};

}
#endif 
