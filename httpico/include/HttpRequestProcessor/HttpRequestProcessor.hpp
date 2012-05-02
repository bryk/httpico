/**
 =========================================================================================
 File name	: /httpico/include/HttpRequestProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPREQUESTPROCESSOR_HPP_
#define HTTPREQUESTPROCESSOR_HPP_

#include "HttpRequest.hpp"
#include <boost/shared_ptr.hpp>

namespace Httpico {

/*
 * Httpico::HttpRequestProcessor
 */
class HttpRequestProcessor {
public:
	explicit HttpRequestProcessor(HttpRequestPtr httpRequest);
	virtual ~HttpRequestProcessor();
	virtual void process() = 0;
private:
	HttpRequestPtr httpRequest;
};

typedef boost::shared_ptr<HttpRequestProcessor> HttpRequestProcessorPtr;

}
#endif 
