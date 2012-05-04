/**
 =========================================================================================
 File name	: /httpico/include/HttpRequestProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPREQUESTPROCESSOR_HPP_
#define HTTPREQUESTPROCESSOR_HPP_

#include "PionterDefinitions.hpp"

namespace Httpico {

/*
 * Httpico::HttpRequestProcessor
 */
class HttpRequestProcessor {
public:
	explicit HttpRequestProcessor(HttpRequestPtr httpRequest, HttpServer &srv_);
	virtual ~HttpRequestProcessor();
	virtual void process() = 0;
protected:
	HttpRequestPtr httpRequest;
	HttpServer &srv;
};
}
#endif 
