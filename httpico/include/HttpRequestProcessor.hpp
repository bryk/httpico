/**
 =========================================================================================
 File name	: /httpico/include/HttpRequestProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPREQUESTPROCESSOR_HPP_
#define HTTPREQUESTPROCESSOR_HPP_

#include "ForwardDeclarations.hpp"

namespace Httpico {

/*
 * Httpico::HttpRequestProcessor
 */
class HttpRequestProcessor {
public:
	explicit HttpRequestProcessor(HttpRequest *);
	virtual ~HttpRequestProcessor();
	void process();
private:
	HttpRequest *httpRequest_;
};

}
#endif 
