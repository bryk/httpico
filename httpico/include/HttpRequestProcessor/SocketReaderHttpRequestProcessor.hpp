/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor/SocketReaderHttpRequestProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef SOCKETREADERHTTPREQUESTPROCESSOR_HPP_
#define SOCKETREADERHTTPREQUESTPROCESSOR_HPP_

#include "HttpRequestProcessor.hpp"

namespace Httpico {

/*
 * Httpico::SocketReaderHttpRequestProcessor
 */
class SocketReaderHttpRequestProcessor: public Httpico::HttpRequestProcessor {
public:
	SocketReaderHttpRequestProcessor(HttpRequestPtr httpRequest);
	virtual ~SocketReaderHttpRequestProcessor();
	virtual void process();
};

}
#endif 
