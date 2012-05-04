/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequestProcessor/HttpRequestProcessor.hpp"

namespace Httpico {

HttpRequestProcessor::HttpRequestProcessor(HttpRequestPtr httpRequest_, HttpServer &srv_) :
		httpRequest(httpRequest_), srv(srv_) {
}

HttpRequestProcessor::~HttpRequestProcessor() {
}

}
