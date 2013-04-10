/**
 =========================================================================================
 File name	: /httpico/include/HttpResponseProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPRESPONSEPROCESSOR_HPP_
#define HTTPRESPONSEPROCESSOR_HPP_

#include "ForwardDeclarations.hpp"
#include "HttpResponseState.hpp"
#include <string>
#include <exception>

namespace Httpico {

typedef std::string Buffer;

/*
 * Httpico::HttpResponseProcessor
 */
class HttpResponseProcessor {
public:
	explicit HttpResponseProcessor(HttpResponse &resp, HttpRequest &request, HttpServerConfiguration &conf);
	virtual ~HttpResponseProcessor();

	void process();
protected:
	virtual Buffer *getContent() throw (std::exception);
	virtual std::string getContentType();
	HttpResponse &response;
	HttpRequest &request;
	HttpServerConfiguration &configuration;
};

}
#endif 
