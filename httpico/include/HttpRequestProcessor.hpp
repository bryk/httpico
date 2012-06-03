/**
 =========================================================================================
 File name	: /httpico/include/HttpRequestProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPREQUESTPROCESSOR_HPP_
#define HTTPREQUESTPROCESSOR_HPP_

#include "ForwardDeclarations.hpp"
#include <string>

namespace Httpico {

/*
 * Httpico::HttpRequestProcessor
 */
class HttpRequestProcessor {
public:
	explicit HttpRequestProcessor(HttpRequest *, HttpResponse *, HttpServerConfiguration &configuration);
	virtual ~HttpRequestProcessor();
	void handleRequest();
	void process();
private:
	HttpRequest *httpRequest;
	HttpResponse *httpResponse;
	HttpServerConfiguration &configuration;

	bool parseRequest(const std::string &buf);
	bool parseResourceName(const std::string &res);
};

}
#endif 
