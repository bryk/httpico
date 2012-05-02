/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor/SocketReaderHttpRequestProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequestProcessor/SocketReaderHttpRequestProcessor.hpp"
#include <iostream>

namespace Httpico {

SocketReaderHttpRequestProcessor::SocketReaderHttpRequestProcessor(HttpRequestPtr httpRequest) :
		HttpRequestProcessor(httpRequest) {
}

SocketReaderHttpRequestProcessor::~SocketReaderHttpRequestProcessor() {
}

void SocketReaderHttpRequestProcessor::process() {
	std::cerr << "process\n";
}

}
